

# Pixel Dynamics Introduction

---

+ 此插件原名Advanced Displacement，字面意思，就是高级的，拥有更多功能的置换插件，具体效果可在开发者b站的主页https://space.bilibili.com/19552499以及随插件附带的实例工程里查看
+ 此版本更新内容会在下方单独介绍，而没有变化的参数请参考此前的插件简介http://www.yuelili.com/ae/plugin-tutorial/advanced-displacement.html
+ 此插件仅支持8&16bpc，不支持32bpc，以及建议使用2017及以上版本ae
+ ~~源码在此https://github.com/dolag233/Advanced-Displacement-Plug-ins-for-After-Effects，不大会写c/cpp以及带了很多没用的注释，见谅~~
+ ~~关于水印，本来想整个彩蛋解谜来解锁水印的，想了想也没什么人就算了，总之~~点击插件上方的*Water Mark Toggle*一次可以消除水印

---

下面是一些新功能的介绍

## Dynamics模式

![dynamics params](G:\ae 插件 工程\ae 插件 笔记 版本信息\Intro\dynamics params.jpg)

在原有的四个模式下增加了Dynamics这个模式，使得像素能够通过置换map进行任意方向的移动

### 关于置换map及原理

**map的red和green通道：**读取map的red，green通道作为x，y轴移动的方向参数，具体来说是，red通道为0时x轴方向分量向右拉满，为255时向左拉满，而为127或128时x分量几乎为0，也就是没有x方向上的位移，相对的，green通道为0时y方向分量向下拉满，为255时向上拉满，为127或128时y轴方向几乎为0，以上情况是在8bpc的情况下的值，而在16bpc情况下是0和32767为负，正向拉满，而值为16384或16383时为0。

**map的blue通道：**读取blue通道的值作为移动或者说置换的长度系数

**原理：**像素会有条件地进行多次置换，每次置换会读取*置换后的最终位置*在map中对应位置的像素的rgb通道值，用于计算下一次置换的最终位置，循环多次得到最终置换位置

---

## 原理图示

<img src="G:\ae 插件 工程\ae 插件 笔记 版本信息\Intro\src.jpg" alt="src" style="zoom:33%;" />

<center>上图为原图


<img src="G:\ae 插件 工程\ae 插件 笔记 版本信息\Intro\map1.jpg" style="zoom:33%;" />

<center>例如上图是置换map(8bpc情况下)，上下左右中的颜色分别是(127,0.255),(127,255,255),(0,127,255),(255,127,255),(127,127,0),也就是上面部分向下置换，下面部分向上置换，左边部分向右置换，右边部分向左置换，中间部分不进行置换(因为blue分量为0)的收缩效果


<img src="G:\ae 插件 工程\ae 插件 笔记 版本信息\Intro\result1.jpg" alt="result1" style="zoom: 50%;" />

<center>上图是置换后的结果


![result2](G:\ae 插件 工程\ae 插件 笔记 版本信息\Intro\result2.jpg)

<center>将置换map给模糊一下，就做好了如上图的向中间收缩的置换效果


---

原理非常简单，但操作起来不是很直观，建议参考示例文件

### 效果面板的其他相关参数

**Displace Velocity：**直译过来就是置换的速度，这一参数决定了置换的循环中每一次置换的距离，也就是说，此参数值越大，每次置换中像素移动的距离越远，或者说移动的步长越大

**Displace Max Iterations：**如前文所说，Dynamics模式是要经过多次循环的置换，此参数控制了最大的循环或者说迭代次数，值越大，Velocity不变的情况下，像素移动的次数越多，距离越远，不过相应的，计算时间也会越长

**Displace Max Distance：**字面意思，就是单个像素移动的最远距离(的平方和)，数值越小，像素置换的最大距离也就越短

## 抗锯齿

勾选Apply Anti-aliasing就会开启抗锯齿(默认开启)，下面的Anti-aliasing Threshold是调整抗锯齿的阈值，阈值越低，应用抗锯齿效果的像素就会越多，越高，则只有非常尖锐的地方会抗锯齿(其实就是模糊orz，效果还不大星)

## Repeat回绕模式

Edge Repeat里有Repeat和Mirror模式，前者是普通平铺，后者是镜像平铺，对应着Motion Tile中的两种模式

---

~~需要注意的是，此插件遵守Apache开源协议~~

~~以及因为本心是想借写插件来锻炼下cpp的技能(结果发现全是用的c那一套)，写的全是垃圾代码，见谅~~

