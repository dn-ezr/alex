#ifndef __alex_regex_cpp__
#define __alex_regex_cpp__

#include "alex.internal.hpp"

namespace alex {

regex regex::extract( std::istream& is, bool inner ) {
    int state = 1;
    bool stay = false;
    int escape_target;
    regex reg;

    while( state > 0 ) {

        const auto pre = is.peek();

        switch( state ) {
            case 1: switch( pre ) {
                case EOF: throw std::runtime_error("unexpected EOF");
                case '/': reg.type = type_t::none; stay = true; state = 0; break;
                case ']': case ')': throw std::runtime_error("unexpected terminator");
                case '(':
                    if( inner ) {
                        reg.type = type_t::character;
                        reg.value = pre;
                        state = 8;
                    } else {
                        state = 2;
                    } break;
                case '\\': 
                    state = 3;
                    reg.type = type_t::character;
                    break;
                case '[': 
                    if( inner ) {
                        reg.type = type_t::character;
                        reg.value = pre;
                        state = 8;
                    } else {
                        state = 6;
                    } break;
                case '.':
                    reg.type = type_t::family;
                    reg.value = (int)family_t::any;
                    state = 8;
                    break;
                default:
                    reg.type = type_t::character;
                    reg.value = pre;
                    state = 8;
                    break;
            } break;
            case 2: {
                reg = compile(is, ')' );
                switch( is.peek() ) {
                    case ')': state = 8; break;
                    default: throw std::runtime_error("terminator expected");
                }
            } break;
            case 3: switch( pre ) {
                case 'x': state = 4; break;
                case 'n': case 'r': case 't': 
                case '\\': case '-': case '/':
                case '?': case '+': case '|': case '.':case '*':
                case '(':case ')':case '[':case ']': {
                    switch( pre ) {
                        case 'n': reg.value = '\n'; break;
                        case 'r': reg.value = '\r'; break;
                        case 't': reg.value = '\t'; break;
                        default: reg.value = pre; break;
                    }
                    state = 8;
                } break;
                case 's': case 'd': case 'p': case 'l': case 'u': case 'b': {
                    switch( pre ) {
                        case 's': reg.value = (int)family_t::space; break;
                        case 'd': reg.value = (int)family_t::digit; break;
                        case 'p': reg.value = (int)family_t::punct; break;
                        case 'l': reg.value = (int)family_t::lower; break;
                        case 'u': reg.value = (int)family_t::upper; break;
                        case 'b': 
                            if( inner ) throw std::runtime_error("boundary not supported inner collection");
                            else reg.value = (int)family_t::boundary;
                            break;
                    }
                    reg.type = type_t::family;
                    state = 8;
                } break;
                default: throw std::runtime_error("unexpected escape character");
            } break;
            case 4: switch( pre ) {
                case '0' ... '9': reg.value = (pre - '0') << 4; break;
                case 'a' ... 'f': reg.value = (pre - 'a' + 10) << 4; break;
                case 'A' ... 'F': reg.value = (pre - 'A' + 10) << 4; break;
                default: throw std::runtime_error("unexpected escape hex");
            } state = 5; break;
            case 5: switch( pre ) {
                case '0' ... '9': reg.value += pre - '0'; break;
                case 'a' ... 'f': reg.value += pre - 'a' + 10; break;
                case 'A' ... 'F': reg.value += pre - 'A' + 10; break;
                default: throw std::runtime_error("unexpected escape hex");
            } state = 8; break;
            case 6: switch( pre ) {
                case '^': reg.type = type_t::reverse; state = 7; break;
                default: reg.type = type_t::collection; stay = true; state = 7; break;
            } break;
            case 7: switch( pre ) {
                case ']': state = 8; break;
                default: reg.content << extract(is, true); stay = true; break;
            } break;
            case 8: if( inner ) {
                if( pre == '-' ) {
                    if( reg.type != type_t::character ) throw std::runtime_error("unexpected dash inner collection");
                    reg.content << reg;
                    reg.type = type_t::range;
                    state = 9;
                } else {
                    stay = true;
                    state = 0;
                }
            } else switch( pre ) {
                case '?': reg = regex{content:{reg},type: type_t::optional}; break;
                case '*': reg = regex{content:{reg},type: type_t::any}; break;
                case '+': reg = regex{content:{reg},type: type_t::more}; break;
                case '|': reg = regex{content:{reg},type: type_t::options}; state = 10; break;
                default: state = 0; stay = true; break;
            } break;
            case 9: {
                reg.content << extract(is, true);
                if( reg.content[1].type != type_t::character ) throw std::runtime_error("unexpected expression for range");
                stay = true;
                state = 0;
            } break;
            case 10: {
                auto right = extract(is);
                if( right.type == type_t::options ) reg.content += right.content;
                else reg.content << right;
                stay = true;
                state = 0;
            } break;
        }

        if( stay ) stay = false;
        else is.get();
    }

    return reg;
}

regex regex::compile( std::istream& is, char terminator ) {

    regex reg;
    reg.type = type_t::sequence;

    while( is.peek() != terminator ) {
        auto item = extract(is);
        reg.content << item;
    }

    if( reg.content.size() == 0 ) reg.type = type_t::none;
    else if( reg.content.size() == 1 ) reg = reg.content[0];

    return reg;
}

std::tuple<int,std::set<int>> regex::attach( fsm& machine, int start ) {
    command_desc::init();
    auto go = tree::reach(root, (char*)"goto");
    auto into = tree::reach(root, (char*)"into");
    long end = 0;
    std::set<int> ends;
    machine[start];
    std::function<std::set<int>(int,std::set<int>,std::set<int>)> proc = [&]( int e, std::set<int> es, std::set<int> history ) {
        e = machine.forword(e);
        std::set<int> ret;
        if( e ) machine[e][-4] = {{go,{(long)start}}};
        for( auto ae : es ) {
            ae = machine.forword(ae);
            if( history.count(ae) ) continue;
            history.insert(ae);
            if( machine[ae].count(-2) ) ret.insert(ae);
            else machine[ae][-2] = {{go,{(long)start}}};
            auto [ie, ies] = content[0].attach( machine, ae);
            ret.merge(proc( ie, ies, history ));
        }
        return ret;
    };
    switch( type ) {
        case type_t::character: {
            if( auto out = machine.findout(start, value); out ) {
                ends.insert(out);
            } else if( auto out = machine.findout(start, -8); out and value >= '0' and value <= '9' ) {
                end = machine.genstate();
                machine[start][value] = {{into,{end}}};
                machine[end][-2] = {{go,{(long)out}}};
            } else if( auto out = machine.findout(start, -7); out and value >= 'A' and value <= 'Z' ) {
                end = machine.genstate();
                machine[start][value] = {{into,{end}}};
                machine[end][-2] = {{go,{(long)out}}};
            } else if( auto out = machine.findout(start, -6); out and value >= 'a' and value <= 'z' ) {
                end = machine.genstate();
                machine[start][value] = {{into,{end}}};
                machine[end][-2] = {{go,{(long)out}}};
            } else if( auto out = machine.findout(start, -5); out ) {
                end = machine.genstate();
                machine[start][value] = {{into,{end}}};
                machine[end][-2] = {{go,{(long)out}}};
            } else {
                end = machine.genstate();
                machine[start][value] = {{into, {end}}};
            }
        } break;
        case type_t::family: {
            machine[end = machine.genstate()];
            auto prog = fsm_program{{into,{end}}};
            auto reg = regex{type: type_t::character};
            auto patch = [&](int i) {
                if( !machine[start].count(i) ) return;
                auto ep = machine.findexit(start,i);
                if( !ep ) return;
                auto& exit = std::get<1>(*ep);
                if( !exit.size() ) return;
                ends.insert((long)exit[0]);
            };
            switch( (family_t)value ) {
                case family_t::any:
                    if( machine[start].count(-5) ) patch(-5);
                    else machine[start][-5] = prog;
                    for( int i = 1; i < 128; i++ ) patch(i);
                    break;
                case family_t::space:
                    if( machine[start].count(-9) ) patch(-9);
                    else machine[start][-9] = prog;
                    for( auto i : {' ','\n','\t','\r'} ) patch(i);
                    break;
                case family_t::digit:
                    if( machine[start].count(-8) ) patch(-8);
                    else machine[start][-8] = prog;
                    for( auto i = '0'; i <= '9'; i++ ) patch(i);
                    break;
                case family_t::punct: {
                    for( auto input : {
                        '`','~','!','@','#','$','%','^','&','*','-','+',
                        '[',']','(',')','{','}','<','>','|',
                        '\\','"','\'',':',';',',','.','?','/'} ) {
                        reg.value = input;
                        reg.attach(machine, start);
                    }
                } break;
                case family_t::lower:
                    if( machine[start].count(-6) ) patch(-6);
                    else machine[start][-6] = prog;
                    for( auto i = 'a'; i <= 'z'; i++ ) patch(i);
                    break;
                case family_t::upper:
                    if( machine[start].count(-7) ) patch(-7);
                    else machine[start][-7] = prog;
                    for( auto i = 'A'; i <= 'Z'; i++ ) patch(i);
                    break;
                case family_t::boundary: throw std::runtime_error("translation for boundary is not supported yet");
            }
        } break;
        case type_t::optional: {
            auto [e,es] = content[0].attach( machine, start);
            for( auto e : es )
                if( !machine[e].count(-2) ) machine[e][-2] = {{go,{(long)e}}};
            if( e ) machine[start][-2] = {{go,{(long)e}}};
            end = e;
        } break;
        case type_t::any: {
            end = start;
            auto [e,es] = content[0].attach( machine, start);
            ends = proc(e, es, {});
        } break;
        case type_t::more: {
            auto [be,bes] = content[0].attach( machine, start);
            end = start = be;
            auto [e,es] = content[0].attach( machine, start);
            es.merge(bes);
            ends = proc(e, es, {});
        }break;
        case type_t::sequence: {
            for( auto expr : content ) {
                std::set<int> next;
                auto [e, es] = expr.attach( machine, start);
                end = e;
                next.merge(es);
                for( auto start : ends ) {
                    auto [e,es] = expr.attach(machine, start);
                    if( e ) machine[e][-4] = {{go,{(long)end}}};
                    next.merge(es);
                }
                ends = next;
                start = end;
            }
        } break;
        case type_t::reverse: {
            for( auto expr : content )
                expr.attach(machine, start);
            if( machine[start].count(-5) )
                ends.insert(machine.findout(start, -5));
            else {
                end = machine.genstate();
                machine[start][-5] = {{into,{end}}};
            }
        } break;
        case type_t::collection: {
            machine[end = machine.genstate()];
            for( auto expr : content ) {
                auto [me, mids] = expr.attach(machine, start);
                machine[me][-4] = {{go,{end}}};
                ends.merge(mids);
            }
        } break;
        case type_t::range: {
            auto reg = regex{type:type_t::character};
            machine[end = machine.genstate()];
            for( auto i = content[0].value; i <= content[1].value; i++ ) {
                reg.value = i;
                auto [me, mids] = reg.attach(machine, start);
                machine[me][-4] = {{go,{end}}};
                ends.merge(mids);
            }
        } break;
        case type_t::options: {
            machine[end = machine.genstate()];
            for( auto expr : content ) {
                auto [me,mids] = expr.attach(machine, start);
                machine[me][-2] = {{go,{end}}};
                ends.merge(mids);
            }
        } break;
        case type_t::none: break;
    }

    machine[end];
    for( auto e : ends ) machine[e];
    return {end,ends};
}

std::tuple<int,std::set<int>> regex::attach( fsm& machine, int start, int accept ) {
    command_desc::init();
    auto cmd_accept = tree::reach(root, (char*)"accept-");
    auto [reach,branchs] = attach( machine, start );
    if( reach == 0 and branchs.empty() ) return {0,{}};
    if( !machine.count(-2) ) machine[reach][-2] = {{cmd_accept, {1L, (long)accept}}};
    for( auto br : branchs )
            if( !machine[br].count(-2) ) machine[br][-2] += {{cmd_accept, {1L, (long)accept}}};
    if( start == 1 ) machine.optimize();
    return {reach, branchs};
}

void regex::print( std::ostream& os ) {
    switch( type ) {
        case type_t::character:
            switch( value ) {
                case '\n': os << "\\n"; break;
                case '\r': os << "\\r"; break;
                case '\t': os << "\\t"; break;
                case '\\':case '/':case '-':
                case '?':case '+':case '|':case '.':case '*':
                case '(':case ')':case '[':case ']': os << '\\' << (char)value; break;
                default: os << (char)value; break;
            } break;
        case type_t::family:
            switch( value ) {
                case (int)family_t::any: os << "."; break;
                case (int)family_t::space: os << "\\s"; break;
                case (int)family_t::digit: os << "\\d"; break;
                case (int)family_t::punct: os << "\\p"; break;
                case (int)family_t::lower: os << "\\l"; break;
                case (int)family_t::upper: os << "\\u"; break;
                case (int)family_t::boundary: os << "\\b"; break;
            } break;
        case type_t::optional: {
            auto surround =  content[0].content.size() > 1;
            if( surround ) os << "(";
            content[0].print(os);
            if( surround ) os << ")";
            os << "?";
        } break;
        case type_t::any: {
            auto surround =  content[0].content.size() > 1;
            if( surround ) os << "(";
            content[0].print(os);
            if( surround ) os << ")";
            os << "*";
        } break;
        case type_t::more: {
            auto surround =  content[0].content.size() > 1;
            if( surround ) os << "(";
            content[0].print(os);
            if( surround ) os << ")";
            os << "+";
        } break;
        case type_t::sequence: {
            for( auto expr : content ) expr.print(os);
        } break;
        case type_t::reverse: {
            os << "[^";
            for( auto expr : content ) expr.print(os);
            os << "]";
        } break;
        case type_t::collection: {
            os << "[";
            for( auto expr : content ) expr.print(os);
            os << "]";
        } break;
        case type_t::range: {
            content[0].print(os);
            os << '-';
            content[1].print(os);
        } break;
        case type_t::options:
            for( int i = 0; i < content.size(); i++ ) {
                if( i != 0 ) os << "|";
                content[i].print(os);
            } break;
    }
}

std::ostream& operator << ( std::ostream& os, regex& reg ) {
    reg.print(os);
    return os;
}

}

#endif