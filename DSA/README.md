DSA 基本原理引用 ctf-wiki 的[相关内容](https://ctf-wiki.github.io/ctf-wiki/crypto/signature/dsa/) 

L，N 的选取为 (1024, 160)。大素数的生成选取 openssl 库。

由于需要满足 $q|p-1$，openssl 库的生成素数函数可以进行相关的实现。

上式可以写成相应的等价形式：$q|p-1 <=> p \equiv 1\ mod \ q$。因此我们首先生成 160 位的素数再生成 1024 位的素数。

选取完 p，q 之后，我们要选 g。g 满足条件 $g=h^{\frac{p-1}{q}}\ mod \ p$。其中 $1<h<p-1$。我们可以通过选取随机数 h 计算出 g。