# Alex

**alex**项目旨在开发一款带运算能力的有限状态机。

- [Alex](#alex)
  - [特性](#%e7%89%b9%e6%80%a7)
  - [实例](#%e5%ae%9e%e4%be%8b)
  - [语法](#%e8%af%ad%e6%b3%95)
    - [基础状态机](#%e5%9f%ba%e7%a1%80%e7%8a%b6%e6%80%81%e6%9c%ba)
    - [合并文法](#%e5%90%88%e5%b9%b6%e6%96%87%e6%b3%95)
    - [默认指令](#%e9%bb%98%e8%ae%a4%e6%8c%87%e4%bb%a4)
    - [多重输入](#%e5%a4%9a%e9%87%8d%e8%be%93%e5%85%a5)
    - [范围输入](#%e8%8c%83%e5%9b%b4%e8%be%93%e5%85%a5)
    - [合并状态](#%e5%90%88%e5%b9%b6%e7%8a%b6%e6%80%81)
    - [文档结束](#%e6%96%87%e6%a1%a3%e7%bb%93%e6%9d%9f)
    - [文法规则](#%e6%96%87%e6%b3%95%e8%a7%84%e5%88%99)
    - [输入规则](#%e8%be%93%e5%85%a5%e8%a7%84%e5%88%99)
    - [变量](#%e5%8f%98%e9%87%8f)
  - [指令集](#%e6%8c%87%e4%bb%a4%e9%9b%86)

## 特性

- Alex可以用于加载带运算有限状态机，并在一个输入流上运行这个有限状态机。

- Alex可以跟踪有限状态机的行为，输出有限状态机接收的词法序列，捕捉有限状态机的输出。

## 实例

下列文法将英文单词归类为`1`记号，将空白符归类为`2`记号，将数字归类为`3`记号，将其他归为`4`记号。

~~~
2, 3, 4 > load tag state
2, 3 * goto 1

1 s, d, * accept- 1
2 s stay
3 d stay

1 (
    > accept- tag
    l, u stay 
    s goto 2
    d goto 3
    * goto 4
)
4 ( * stay l, u, d, s goto 1 )
1, 2, 3, 4 e (accept- state end)
~~~

## 语法

### 基础状态机

Alex状态机从`1`状态开始运行，描述此状态下的行为即可开始`Alex`程序。

~~~
1 'a' stay
1 'b' accept 1
1 'b' end
~~~

上述文法的第一行表示在`1`状态，若输入`a`字符，则停滞在此状态。

第二行连同第三行表示若在1状态输入`b`字符，则接受当前token并标记为`1`，然后结束有限状态机运行。

上述文法和正则表达式`a*b`的效果相似。

### 合并文法

上述文法中的第二和第三行所表述的指令都在`1`状态输入`b`字符时按照书写顺序依次执行，所以我们可以将相同的部分合并：

~~~
1 'a' stay
1 'b' ( 
    accept 1 
    end
)
~~~

类似的，我们也可以合并同一个状态上的不同输入:

~~~
1 (
    'a' stay
    'b' ( accept 1 end )
)
~~~

### 默认指令

若您希望为无法预想的输入设计指令，请使用`256`或`*`作为输入情况，当其他情况都不匹配时，有限状态机将执行`256`输入下的内容。

~~~
1 (
    'a' stay
    'b' stay
    'c' stay
    'z' stay
    * ( accept 1 end )
)
~~~

### 多重输入

若多种输入的对应指令相同，至少有一部分相同，您可以将多种输入情况合并:

~~~
1 (
    'a', 'b', 'c', 'z' stay
    * ( accept 1 end )
)
~~~

### 范围输入

若多种输入可以联合成为一个范围，则您可以将其描述为一个范围来节省笔墨：

~~~
1 (
    'a' ~ 'c', 'z' stay
    * ( accept 1 end )
)
~~~

### 合并状态

若某些状态对某些输入的部分指令是相同的，您可以将多重状态书写在一个文法上：

~~~
1, 2, 3 (
    'a' ~ 'c', 'z' stay
    * ( accept 1 end )
)
~~~

### 文档结束

文档结束时，将输入一个EOF,在文法中，可以使用`e`来表示文件结束时的情况。

~~~
1, 2, 3 (
    'a' ~ 'c', 'z' stay
    * ( accept 1 )
    e end
)
~~~

### 文法规则

请理解，**Alex**的每一行都在为某（些）状态下的某（些）输入情况**添加**指令。**Alex**源代码都不会覆盖之前书写过的指令。

只要设计好书写顺序，您可以编排复杂的重叠文法。

在一个输入上，您可以按照顺序书写多条指令，它们会按照书写顺序被执行，直到此状态上的指令被耗尽或执行了**状态指令**。

### 输入规则

Alex以字节为单位处理输入流，但是Alex提供了更多的虚拟输入，用于处理其他情况或便捷处理：

|输入名|输入值|解释|
|---|---|---|
|`e`|`-1`|用于处理输入EOF时的情况|
|`*`|`-2`|用作默认输入，当输入与任何其他情况都不匹配时使用|
|`-`|`-3`|首次进入此状态时执行|
|`>`|`-4`|每次从其他状态进入此状态时执行|
|`l`|`a-z`|所有的小写字母|
|`u`|`A-Z`|所有的大写字母|
|`d`|`0-9`|所有的数字|
|`s`|` \n\t\r`|空格，制表符，回车，换行|

### 变量

在Alex虚拟机中内置了可以动态配置的变量空间，变量名称完全由英文字母构成。有些指令可以用来访问这些变量。

其中有些变量是特殊的“静态变量”它们通常用于反应系统当前的状态，在每次进入一个状态时，这些变量就会被系统修正为当前的状态。

您可以访问它们，利用它们临时存储数据，但是它们不能用于跨越状态而保持某个自定义的值。

|变量名|类型|说明|
|---|---|---|
|`state`|`integer`|当前状态|
|`pre`|`integer`|当前预览，文本流输入时，等同于当前预览文本|
|`pre.tx`|`string`|当前预览的文本|
|`pre.begl`|`integer`|当前预览的起始行|
|`pre.begc`|`integer`|当前预览的起始列|
|`pre.endl`|`integer`|当前预览的终止行|
|`pre.endc`|`integer`|当前预览的终止列|
|`buf`|`string`|当前缓冲的文本|
|`buf.begl`|`integer`|当前缓冲的起始行|
|`buf.begc`|`integer`|当前缓冲的起始列|
|`buf.endl`|`integer`|当前缓冲的终止行|
|`buf.endc`|`integer`|当前缓冲的终止列|

## 指令集

`Alex`程序的指令格式类似于汇编程序，由`cmd [arg]...`的格式构成一条指令。

下表描述了`Alex`支持的所有指令，参数列表中的每个字母表示一种数据类型：

 - i: 整数，可以使用数字或单引号包括的字符，字符支持C语言风格转义。
 - s: 字符串，使用双引号包含的文本内容，支持转义。
 - a: 整数或字符串。
 - l: 标记，由英文字母或下划线开头，由英文字母，数字，下划线和点构成的单词，或由数字构成的地址。
 - n: 名称，由英文字母或下划线开头，由英文字母，数字，下划线和点构成的单词。
 - p: 一个标点，可以包含连续的多个字符。

|指令名|参数列表|备注|
|---|---|---|
|`end`||结束有限状态机|
|`drop`||放弃当前预览，停滞在当前状态|
|`input`||消耗当前预览，输入到当前缓冲，停留在当前状态|
|`into`|`i`|消耗当前预览，输入到当前缓冲，进入`%0`状态|
|`goto`|`i`|不处理当前预览，进入`%0`状态|
|`stay`|`i`|将`%0`个输入压入当前状态的词汇容器，停留在当前状态|
|`accept+`|`ll`|将当前预览消耗，输入到当前缓冲。将当前缓冲接受为`%0`。跳转到`%1`状态|
|`accept-`|`i`|若当前缓冲不为空，将当前缓冲接受为`%0`。跳转到`%1`状态。|
|`mem.init`|`i`|初始化虚拟内存空间，长度为`%0`字节|
|`store`|`la`|将`%1`数字或字符串存入`%0`地址或变量。|
|`store.len`|`ls`|将`%1`字符串的长度存入`%0`地址或变量。|
|`load`|`nn`|将`%1`赋值给`%0`|
|`eval`|`nnpn`|解算表达式，`%0`用于存储结果。<br/>`%1`和`%3`是运算数，`%2`是运算符。<br/>支持的运算符如下:<br/>`+` 将数字相加<br/>`-` 将数字相减<br/>`*` 将数字相乘<br/>`/` 将数字相除<br/>`%` 将数字相余<br/>`.` 将字符串相连接<br/>`&` 将数字相与<br/>`|` 将数字相或<br/>`^` 将数字异或|
|`j.cmp`|`nnpn`|判断条件是否成立，并决定是否跳转到目标锚点执行程序。<br/>`%1`和`%3`使用`%2`进行比较，若结果成立，跳转到`%0`，支持的比较如下：<br/>`<` 判断小于<br/>`>` 判断大于<br/>`>=` 判断大于等于<br/>`<=` 判断小于等于<br/>`!=` 判断不等于<br/>`==` 判断等于<br/>|
|`@`|`n`|设定一个锚点，无其他动作|