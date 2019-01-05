安装 open-ssl 开发环境

```bash
sudo apt-get install libssl-dev
```

Makefile 样例

```makefile
CC = g++
LDFLAGS = -L/usr/local/opt/openssl/lib
CPPFLAGS = -I/usr/local/opt/openssl/include

all: binary

binary: main.cpp
	$(CC) main.cpp -o binary -lssl -lcrypto $(LDFLAGS) $(CPPFLAGS)

rm:
	rm binary *.o
```

WSL C++环境配置

```json
{
    "configurations": [
         {
             "name": "WSL",
             "intelliSenseMode": "clang-x64",
             "compilerPath": "/usr/bin/gcc",
             "includePath": [
                 "${workspaceFolder}",
                 "/usr/include/"
             ],
             "defines": [],
             "browse": {
                 "path": [
                     "${workspaceFolder}",
                     "/usr/include"
                 ],
                 "limitSymbolsToIncludedHeaders": true,
                 "databaseFilename": "",
             },
             "cStandard": "c11",
             "cppStandard": "c++17"
         }

    ],
    "version": 4
}
```

openssl 生成素数

```c
int BN_generate_prime_ex(BIGNUM *ret,int bits,int safe, const BIGNUM *add,
     const BIGNUM *rem, BN_GENCB *cb);
```



tricks:

p、q 的选取位数为输入字符串长度的三倍。

e 的选取为输入字符串长度四倍的素数。

由于有 $ed\equiv 1(mod\ r)$ ，计算 d 的操作使用欧几里得扩展算法。

上式可以转化为 $ed-rx=1$ 。其中 $e$，$r$ 已知且 $r>>e$。

进一步转化为 $rx+ed=1$ 。

如果用 Python 实现欧几里得扩展算法，其形式可以被描述如下：

```python
def exp_euclid(a,b):
    if b == 0:
        return 1, 0, a
    else:
        x, y, q = exp_euclid(b, a%b)
        x, y = y, (x-(a//b)*y)
        return x, y, q
```

输入值是 $a$、$b$。输出值是 $x$、$y$、$q$。

使用 C 语言实现的话形式如下：

```c
int gcdEx(int a, int b, int *x, int *y) 
 {
     if(b==0)
     {
         *x = 1,*y = 0;
         return a;
     }
     else
     {
         int r = gcdEx(b, a%b, x, y);
         int t = *x;
         *x = *y;
         *y = t - a/b * *y;
         return r;
     }
 }
```

在这里我用带有 openssl `BIGNUM` 扩展的 C++ 实现。