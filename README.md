# My_RegexEngine
Ps：
    在2015年1月寒假的前半段，在大二上学期自学编译原理的过程中，我学习了有关正则表达式的内容，并利用寒假实现了这个不太完善的第一个正则表达式引擎，代码有1K行。
<h1>过程 ：</h1>
        Re->NFA->DFA
<h2>具体算法：</h2>
        Re->NFA:
        利用Thompson算法。
        NFA->DFA:
        利用子集构造法。
<h2>缺陷 ：</h2>
        因为在理解Hopcroft算法后，始终无法将实现，我并没有将DFA最小化，所以这个正则引擎的效率是一个大问题，只支持常
        规的正则表达式，没有其它的特性是一个遗憾，在我深入学习的以后，我还会再实现一个更加完善的正则表达式引擎。
                                                                                写于2015.1.28 凌晨。

