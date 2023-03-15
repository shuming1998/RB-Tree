![image-20230315194143171](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315194143171.png)

![image-20230315194212090](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315194212090.png)

![image-20230315194301665](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315194301665.png)

![image-20230315194424430](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315194424430.png)

![image-20230315194719000](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315194719000.png)

![image-20230315194851698](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315194851698.png)

![image-20230315194902376](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315194902376.png)

![image-20230315200459793](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315200459793.png)



![image-20230315212635933](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315212635933.png)

 

![image-20230315212733315](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315212733315.png)

![image-20230315213043061](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315213043061.png)

只有三种情况：

1. 一个单独的黑色节点就是一个B树2节点
2. 一个黑色节点和一个红色子节点，构成一个B树3节点，可以左倾也可以右倾
3. 一个黑色节点和两个红色子节点，构成一个B树4节点

![image-20230315213402657](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315213402657.png)

#### 左倾：

![image-20230315213450943](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315213450943.png)

右倾：

<img src="C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315213502668.png" alt="image-20230315213502668" style="zoom:80%;" />

![image-20230315213837959](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315213837959.png)



#### 默认新增节点的颜色为红色

![image-20230315215005863](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315215005863.png)

![image-20230315215115551](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315215115551.png)

![image-20230315215923795](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315215923795.png)

![image-20230315220253231](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315220253231.png)

#### 新增节点时，只有两种情况需要调整：

#### 1.  3-节点的添加，无叔节点或叔节点为黑色

#### 2.  4-节点的添加，有叔节点且为红色

![image-20230315221742173](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315221742173.png)

![image-20230315222359568](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315222359568.png)

#### 红黑树删除

![image-20230315224510740](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315224510740.png)

![image-20230315224955954](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315224955954.png)

​		**对于第3种情况，以删除节点10为例，前驱节点为黑色节点4，后继节点为黑色节点12。如果删除4，调整较多，而如果删除12，可以转化为情况2，此时调整较少。所以对于情况三，调整策略为：先找前驱节点，判断是否为情况2，如果不是的话，再找后继节点，判断是否为情况2，如果都不满足情况2，就用后继。**

![image-20230315225424677](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315225424677.png)

**其实只用前驱或者只用后继都可以，但这不失为一个可以优化调整的方法。**

**比如删除节点50**

**前驱节点为45，45是有一个红色叶子节点的黑节点，将45作为50的替代节点，操作步骤为：将50变为45，将45变为42，然后删除红色叶子节点42。**

**而后继节点为60，是一个黑色的叶子节点，将50变为60，然后再删除黑色叶子节点60，此时以80为根节点的子树黑高就不平衡了，比较麻烦。**

![image-20230315231015687](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315231015687.png)

**删除节点时，通过转化，只有两种情况：删除红色叶子节点和删除黑色叶子节点。只有一种情况需要调整，即删除的是黑色叶子节点。**

![image-20230315231916554](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315231916554.png)

![image-20230315232008447](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315232008447.png)

![image-20230315232350405](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315232350405.png)

**如果兄弟有两个红色子节点，当作LL型处理即可**

![image-20230315232721786](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315232721786.png)

![image-20230315232839514](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315232839514.png)

![image-20230315232852647](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315232852647.png)

**对于情况2.2.2，只是把父节点当作要删除的节点进行递归调整，并不真的删除**

**具体操作为：**

![image-20230315233217926](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315233217926.png)

![image-20230315233514516](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315233514516.png)

**这里还是以删除黑色右叶子节点D为例，它的左兄弟B为红，先将兄弟B和右侄NR颜色互换，然后右旋父节点P**

![image-20230315233833587](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315233833587.png)

![image-20230315234131833](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315234131833.png)

![image-20230315234320164](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315234320164.png)

![image-20230315234551019](C:\Users\10055\AppData\Roaming\Typora\typora-user-images\image-20230315234551019.png)