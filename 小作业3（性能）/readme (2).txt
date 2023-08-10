我发现在程序中的void statisWord(WordsStatis& wordsStatis, std::string& word, int lineNo)
函数中存在反复使用new的情况，所以我将new去掉转为在内存中创建对象后加入map中达到同样的效果。
在运行test.txt后原程序的运行时间为0.032577s，改写后的时间为0.025876s，提升大约为21%。

在test.txt中，一共有26483个单词，数量最多的三个单词分别是the，of，a。