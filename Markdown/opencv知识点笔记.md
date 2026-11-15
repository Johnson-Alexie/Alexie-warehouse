## opencv基础



### 一、GaussianBlur 高斯模糊函数

1.主要用途：

降噪 ：抑制高斯噪声，是很多算法的前置步骤


平滑细节 ：去掉高频纹理、细小颗粒


边缘检测前处理 ：Canny 通常先做一步 GaussianBlur，否则噪声会被当成边缘


多尺度/金字塔 ：构建尺度空间、下采样前的抗混叠


削弱振铃和假细节 ：比如 SIFT/SURF 预滤波



代价是：模糊会让边缘变软，σ 越大越糊。

**函数原型**

> `void cv::GaussianBlur(`
>     `InputArray  src,`
>     `OutputArray dst,`
>     `Size        ksize,`
>     `double      sigmaX,`
>     `double      sigmaY = 0,`
>     `int         borderType = BORDER_DEFAULT`
> `);`

| 参数         | 作用                   | 注意点                                                       |
| ------------ | ---------------------- | ------------------------------------------------------------ |
| `src`        | 输入图像               | 支持 `Mat`，多通道会 **逐通道独立** 处理；深度可为 8U/16U/16S/32F/64F |
| `dst`        | 输出图像               | 尺寸、类型与 `src` 相同；可以和 `src` 同一个 `Mat`（内部自动处理） |
| `ksize`      | 高斯核大小 `Size(w,h)` | **必须是正奇数** ，否则抛异常。`w` 和 `h` 可以不同，实现的是可分离的各向异性模糊；设为 `Size(0,0)` 时由 `sigma` 反推核大小 |
| `sigmaX`     | X 方向标准差 σ         | 控制模糊强度。设 0 表示由 `ksize` 自动推算：`sigma = 0.3*((ksize-1)*0.5 - 1) + 0.8` |
| `sigmaY`     | Y 方向标准差 σ         | 设 0 时 **自动等于 sigmaX** （各向同性）。想只在某方向模糊，给另一方向一个很小的值 |
| `borderType` | 边界填充方式           | 默认 `BORDER_DEFAULT`（即 `BORDER_REFLECT_101`）。可改成 `BORDER_REPLICATE`、`BORDER_CONSTANT` 等； **但 GaussianBlur 不支持 `BORDER_WRAP`** |

|                     | 真正控制什么                                          |
| ------------------- | ----------------------------------------------------- |
| **σ（sigma）**      | **模糊强度** ，也就是"糊到什么程度"。它是物理量的本体 |
| **ksize（核大小）** | **能覆盖的模糊范围上限** ，决定"σ 能否被完整表达"     |

σ 控制模糊强度，决定权重的衰减快慢；核大小控制采样窗口能覆盖多大范围，必须与 σ 匹配（约 6σ+1）才能得到准确结果——核小了会截断高斯、模糊不足，核大了只是徒增计算量。


顺带一个实战判断法：想让图像"糊得很明显"就加大 σ，别去 {@ 单纯堆 ksize；如果发现 ksize 已经很大但图像还很清楚，那一定是 σ 没跟上。





### 二、Canny 边缘检测函数

1.Canny 是 边缘检测 算子，输出一张二值图：白色是边缘像素，黑色是背景。


它的定位和 GaussianBlur 正好是一对——GaussianBlur 做平滑，Canny 找梯度突变。Canny 内部第一步就是自己做一次 5×5 高斯平滑，所以 不需要你再手动先模糊一遍 （手动画蛇添足反而会削弱边缘）。


它之所以被当作"经典边缘检测的标准答案"，是因为它提出了三个明确目标并同时满足：



低错误率 ——真实边缘不漏检，非边缘不误检


定位准确 ——检出的边缘点尽量落在真实边缘中心

单像素响应 ——一条边缘只返回一条细线，而不是多条宽响应



**函数原型**

`void cv::Canny(`
    `InputArray  image,`
    `OutputArray edges,`
    `double      threshold1,`
    `double      threshold2,`
    `int         apertureSize = 3,`
    `bool        L2gradient   = false`
`);`

**参数说明**

| 参数           | 作用               | 注意点                                                       |
| -------------- | ------------------ | ------------------------------------------------------------ |
| `image`        | 输入图像           | **必须是 8 位单通道** （`CV_8UC1`），彩色图要先 `cvtColor` 转灰度。传三通道会抛异常 |
| `edges`        | 输出边缘图         | 类型是 `CV_8UC1`，非 0 即 255。尺寸与输入相同                |
| `threshold1`   | **低阈值**         | 用来"接续"弱边缘。边缘梯度高于它、且与强边缘相连的像素会被保留 |
| `threshold2`   | **高阈值**         | 用来"认定"强边缘。梯度高于它的像素直接判定为边缘             |
| `apertureSize` | Sobel 算子的核大小 | 默认 3，可选 3/5/7。越大对噪声越鲁棒，但定位越粗糙。不能用 1 |
| `L2gradient`   | 梯度幅值的计算方式 | `false`（默认）用 |Gx|+|Gy|，快；`true` 用 √(Gx²+Gy²)，更精确但慢 |



阈值是这套算法的灵魂


Canny 的 双阈值 + 滞后连接 （hysteresis）是它区别于普通梯度阈值的关键：



梯度 > threshold2 →  强边缘 ，直接保留


threshold1 < 梯度 < threshold2 →  弱边缘 ，只有和强边缘连通时才保留


梯度 < threshold1 → 直接丢弃



所以两条阈值的分工是： 高阈值负责"起头"，低阈值负责"延伸" 。


经验规则：



threshold1 : threshold2 常取 1:3 到 1:2 ，这个比例比绝对值更重要


绝对值怎么定：先用大致的 2:1 或 3:1 比例试几组，看边缘连续性和噪声量


阈值太高  → 边缘断断续续，漏检


阈值太低  → 噪点全被当边缘，画面脏

手动调太烦的话，OpenCV 有 cv::threshold 配合 Otsu 来自适应求阈值，也可以自己按图像梯度中位数估算





### 三、dilate 加粗函数？

dilate 是 形态学膨胀 ，用结构元素在图像上滑动，取邻域内的 最大值 赋给中心像素。

直观理解：把白色（亮）区域"往外撑大"。白色区域边界向外扩张，暗的细缝、小孔洞会被白色吃掉。



**主要用途：**

连接断裂 ：把 Canny 输出中断开的边缘接上


填充小孔洞、填补裂缝


去除小黑点噪声 （亮背景上的暗噪点）


加粗目标 ：让细线、文字笔画变粗，提高可检测性


求局部极大值 、和一些算子组合做形态学梯度



注意它是 腐蚀（erode）的反操作 ，二者配对使用是一整套形态学工具箱的基础。

**函数原型**

`void cv::dilate(`
    `InputArray      src,`
    `OutputArray     dst,`
    `InputArray      kernel,`
    `Point           anchor = Point(-1,-1),`
    `int             iterations = 1,`
    `int             borderType = BORDER_CONSTANT,`
    `const Scalar&   borderValue = morphologyDefaultBorderValue()`
`);`

**参数说明**

| 参数          | 作用         | 注意点                                                       |
| ------------- | ------------ | ------------------------------------------------------------ |
| `src`         | 输入图像     | 支持多通道， **逐通道独立** 处理。建议 8U 或 32F             |
| `dst`         | 输出图像     | 尺寸类型同 `src`，可与 `src` 相同（原地操作）                |
| `kernel`      | **结构元素** | 决定"撑大"的形状和尺寸。传空 `Mat()` 时默认用 3×3 矩形核     |
| `anchor`      | 核的锚点位置 | 默认 `Point(-1,-1)` 表示取核中心。只有想让膨胀偏向某个方向时才改 |
| `iterations`  | 迭代次数     | 默认 1。 **迭代 N 次 ≈ 用 N 倍大的核膨胀一次** ，但迭代的核必须是能"累加"的形状（矩形、椭圆可以，十字也行），且成本是线性的 |
| `borderType`  | 边界填充方式 | 默认 `BORDER_CONSTANT`                                       |
| `borderValue` | 边界常数值   | 默认 `morphologyDefaultBorderValue()`，对膨胀来说是极小值，等价于"边界外不考虑"，避免图像边缘被伪膨胀 |



**结构元素是最关键的参数**


kernel 用 **cv::getStructuringElement** 生成，三选一：

cv::getStructuringElement(cv::MORPH_RECT,    cv::Size(5,5)); 、**// 矩形，膨胀最"方"**
cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5));   **// 椭圆/圆，膨胀最"圆"**
cv::getStructuringElement(cv::MORPH_CROSS,   cv::Size(5,5));   **// 十字，只沿横竖方向**

**MORPH_RECT ：**默认最常用，膨胀量最均匀，边缘会变方

**MORPH_ELLIPSE ：**圆形元素，膨胀后目标轮廓更接近原形状，不出现明显方角

**MORPH_CROSS ：**只补横竖方向，适合处理水平/垂直断线，比如文字行、表格线



**典型应用场景**


1. **接上 Canny 断掉的边缘**

这是你上一步 Canny 最自然的后续：

`cv::Canny(gray, edges, 50, 150);`
`cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));`
`cv::dilate(edges, edges, kernel);   // 边缘变粗、断口连上`

注意： 先 dilate 再 findContours 更容易得到闭合轮廓 ，但 dilate 会让轮廓外扩，如果要精确测尺寸，建议用 morphologyEx 的 MORPH_CLOSE（先膨胀后腐蚀），既连通又不改变整体尺寸。




2. **配合 erode 做开闭运算**

开运算 （先 erode 后 dilate，MORPH_OPEN）：去小亮斑、去噪，保持大目标尺寸


闭运算 （先 dilate 后 erode，MORPH_CLOSE）：填小孔、接裂缝，保持大目标尺寸



单独用 dilate 会改变目标大小， 要"连通但不胖"就用闭运算 。


3. **加粗文字笔画**


OCR 前处理里，笔画太细导致识别失败时，来一次小核膨胀往往能救回来。

**一个容易踩的坑**


dilate 取的是 最大值 ，所以：

图像是"**白底黑字**"时，膨胀后 黑字会变细、消失 ——因为白（大值）在吞掉黑（小值）


想加粗黑字，得先 bitwise_not 反色，或改用 erode







### 四、erode 函数

cv::erode 是 形态学腐蚀 ，和 dilate 完全对称：用结构元素在图像上滑动，取邻域内的 最小值 赋给中心像素。

直观理解：把白色（亮）区域"往里削瘦"。白色边界向内收缩，暗的小区域、细缝会变大。



**主要用途：**

去除小白点噪声 （亮背景上的亮噪点）


断开细小连接 ：把粘连在一起的目标分开


细化/瘦身目标 ：让粗线变细，轮廓内缩


消除毛刺 ：抹平目标边缘的小凸起


求局部极小值



关键区别记住一句： dilate 取 max 让亮区变大，erode 取 min 让亮区变小。



**函数原型**

`void cv::erode(`
    `InputArray      src,`
    `OutputArray     dst,`
    `InputArray      kernel,`
    `Point           anchor = Point(-1,-1),`
    `int             iterations = 1,`
    `int             borderType = BORDER_CONSTANT,`
    `const Scalar&   borderValue = morphologyDefaultBorderValue()`
`);`

参数和 dilate  一模一样 ，含义也完全对应：

| 参数          | 作用       | 注意点                                           |
| ------------- | ---------- | ------------------------------------------------ |
| `src`         | 输入图像   | 多通道逐通道独立处理                             |
| `dst`         | 输出图像   | 尺寸类型同 `src`，可与 `src` 相同                |
| `kernel`      | 结构元素   | 决定"削瘦"的形状和尺寸；传 `Mat()` 默认 3×3 矩形 |
| `anchor`      | 核的锚点   | 默认 `Point(-1,-1)` 取核中心                     |
| `iterations`  | 迭代次数   | 迭代 N 次 ≈ 用 N 倍大核腐蚀一次                  |
| `borderType`  | 边界填充   | 默认 `BORDER_CONSTANT`                           |
| `borderValue` | 边界常数值 | 默认对腐蚀是"极大值"，避免图像边缘被伪腐蚀       |

结构元素的选择完全一致





### 五、resize 函数

cv::resize 是 图像缩放 ，把输入图像按指定的尺寸或比例重采样成新尺寸。它不改变图像的通道数和数据类型，**只改变像素网格的宽高**。


注意它和另一类操作的区别：



resize：改变 分辨率 （像素数量）


pyrDown/pyrUp：改变分辨率，但 强制按 2 倍 并配合高斯滤波


ROI 裁剪、warpAffine：不改变像素密度，只是截取或几何变换



**主要用途：**

统一输入尺寸 ：神经网络、模板匹配、特征比对都要求固定宽高


降低分辨率提速 ：大图先缩小再处理，Canny、模板匹配快很多


放大用于显示 ：小图放大观察细节


构建图像金字塔 、多尺度检测



**函数原型**

`void cv::resize(`
    `InputArray  src,`
    `OutputArray dst,`
    `Size        dsize,`
    `double      fx = 0,`
    `double      fy = 0,`
    `int         interpolation = INTER_LINEAR`// ← 默认值
`);`

**参数说明**

| 参数            | 作用                                | 注意点                                                       |
| --------------- | ----------------------------------- | ------------------------------------------------------------ |
| `src`           | 输入图像                            | 支持 8U/16U/32F 等多通道，逐通道独立缩放                     |
| `dst`           | 输出图像                            | 尺寸类型由 `dsize` 或 `fx/fy` 决定，可与 `src` 相同          |
| `dsize`         | **目标尺寸**  `Size(width, height)` | 填 `Size(0,0)` 时由 `fx/fy` 反推。 **这是最容易搞错的参数，见下方提醒** |
| `fx`            | **水平方向缩放比例**                | 宽变为 `src.cols * fx`                                       |
| `fy`            | **垂直方向缩放比例**                | 高变为 `src.rows * fy`                                       |
| `interpolation` | **插值方式**                        | 缩放质量的核心，按"缩小/放大"分别选，见下表                  |

**关键规则：dsize 和 fx/fy 二选一**


这两组参数是"一个是显式、一个是隐式"的关系：

dsize 非空  → 用 dsize，fx/fy 被忽略


dsize = Size(0,0)  → 必须给 fx 和 fy，按比例缩放


dsize 和 fx/fy 不能都不给，否则抛异常

| 插值方式             | 适用场景                 | 说明                                                         |
| -------------------- | ------------------------ | ------------------------------------------------------------ |
| `INTER_NEAREST`      | 缩小提速、标签图/掩膜    | 最近邻，最快，会有锯齿和块状失真                             |
| `INTER_LINEAR`       | **默认，放大和一般缩小** | 双线性，速度快质量够用                                       |
| `INTER_AREA`         | **缩小首选**             | 基于像素区域关系，缩小无摩尔纹，实测最清晰；但放大时效果等同于双线性 |
| `INTER_CUBIC`        | 放大追求质量             | 三次样条，细节更锐利，比线性慢                               |
| `INTER_LANCZOS4`     | 高质量放大               | 8×8 邻域 Lanczos，最慢，适合最终输出                         |
| `INTER_LINEAR_EXACT` | 位精确的双线性           | 需要确定性结果时用                                           |

工程记忆法 ：

缩小 → INTER_AREA （面积平均，抗混叠）


放大 → INTER_CUBIC 或 INTER_LINEAR


掩膜/label 图 → 一律 INTER_NEAREST （否则会插出不存在的中值 ID）





### 六、rect结构体

构造 一个对象，用它描述图像上的一个矩形区域。

它的核心定位是  ROI（Region of Interest，感兴趣区域）的载体 ——告诉你"图像上哪一块"，然后用 img(rect) 把这块像素取出来。



**主要用途：**

抠 ROI ：裁剪局部区域做处理


记录检测结果 ：目标框、人脸框、匹配位置


限定处理范围 ：只对某区域滤波/二值化


边界框运算 ：求框的交集、并集、判断包含关系



**构造方式**


Rect 是结构体，用 构造函数 创建，主要有两种重载：

`cv::Rect rect(x, y, width, height);   // 最常用：左上角坐标 + 宽高`
`cv::Rect rect(pt1, pt2);              // 两个对角点，自动整理成左上 + 宽高`
`cv::Rect rect(point, size);           // 点 + 尺寸`

**四个成员（相当于"参数"）**

| 成员     | 类型 | 含义                           |
| -------- | ---- | ------------------------------ |
| `x`      | int  | 左上角 **列** 坐标（水平方向） |
| `y`      | int  | 左上角 **行** 坐标（垂直方向） |
| `width`  | int  | 宽度                           |
| `height` | int  | 高度                           |

坐标约定：x 对应 cols，y 对应 rows。 主体是左上角 + 宽高 ，不是左上角 + 右下角

**三个必须知道的坑 ：**


1. 取 ROI 是浅拷贝。  src(roi) 不复制像素，只指向原图那块内存，改 patch 会改到 src。要独立数据必须 .clone()。


2. 不做越界检查。  Rect 本身不管图像多大，直接 src(roi) 越界会抛 Assertion failed。用 &= 收缩到图像范围内。


3. Rect 不持有像素。  它只是坐标描述，和任何图像无关，必须 img(rect) 才拿到数据。

**一句话总结**


Rect(x, y, width, height) 是 "左上角 + 宽高" 的矩形结构体，不是函数；用它抠 ROI 是 浅拷贝 且 不检查越界 ，所以实战里 **rect & 图像边界 和 .clone() 这两步几乎成了标配**





### 七、HSV 色彩空间

HSV 是把我们 人眼感知颜色 的方式拆成三个维度： 是什么颜色（色相 H）、有多鲜艳（饱和度 S）、有多亮（明度 V） 。它比 RGB 更贴近人的直觉，所以在图像处理里常用来做 颜色分割、调参、取色 。

| 分量      | 英文       | 含义                                       | 取值范围（OpenCV） | 取值范围（常规/色彩学） |
| --------- | ---------- | ------------------------------------------ | ------------------ | ----------------------- |
| H：色相   | Hue        | 颜色的种类，红色、绿色、蓝色所在的角度位置 | 0–180              | 0–360°                  |
| S：饱和度 | Saturation | 颜色的纯度，0 表示灰白，越高越鲜艳         | 0–255              | 0%–100%                 |
| V：明度   | Value      | 亮度，0 表示纯黑                           | 0–255              | 0%–100%                 |

**H（色相）**

本质是角度，在色环上绕一圈 360°：0° 红 → 60° 黄 → 120° 绿 → 180° 青 → 240° 蓝 → 300° 品红 → 回到红。


OpenCV 里把它压缩到 0–180 ：因为 8 位通道存不下 0–360，所以统一除以 2。你写阈值时 H=0~10 实际对应 0°~20°，这点最容易踩坑。



**S（饱和度）**

色环上离中心轴的距离。**S=0 时无论 H 是多少都是灰色**。


用 HSV 做分割时， **调高 S 下限能有效滤掉白墙、灰地这些低饱和背景 。**



**V（明度）**

颜色最亮通道的值。**V=0 是纯黑。**


和亮度（Lightness/Luminance）不同，V 受光照影响很大，暗处的红色 V 会很低。



**常见色相参考表（OpenCV 8 位值）**

| 颜色 | H 范围          |
| ---- | --------------- |
| 红   | 0–10 和 156–180 |
| 橙   | 11–22           |
| 黄   | 23–33           |
| 绿   | 34–77           |
| 青   | 78–99           |
| 蓝   | 100–124         |
| 紫   | 125–155         |





### 八、Scalar 向量

它是 OpenCV 里的一个 四维双精度向量 （cv::Scalar_<double>），本质就是个"能装 4 个 double 的固定小数组"。

**为什么是 4 个？各位置代表什么**

| 通道数                | 各位置含义                         |
| --------------------- | ---------------------------------- |
| 单通道（灰度）        | `[0]` = 灰度值，其余不用           |
| 3 通道（BGR / HSV）   | `[0][1][2]` = 三个通道，`[3]` 不用 |
| 4 通道（BGRA / BGRA） | 四个值全用，`[3]` 是 alpha         |





### 九、前后非零像素数等统计信息

“非零像素数”就是 像素值不为 0 的像素个数 。在灰度图和二值图里，这个数字有明确的物理含义，所以常拿来做统计对比。

**一、先理解“非零”指什么**

灰度图 ：每个像素 0~255。像素值 0 = 纯黑，其余都算“非零”，所以 非零像素数 ≈ 所有非纯黑像素的个数 。


二值图 ：只有 0 和 255 两种值。255 = 前景（白），0 = 背景（黑），所以 非零像素数 = 前景像素总数 。


掩膜（mask） ：非零 = 被选中的像素。

countNonZero(mat) 就是把上面这些数一遍，只用一行。

| 统计量       | 含义                  | 用途                         |
| ------------ | --------------------- | ---------------------------- |
| 非零像素数   | 前景/亮区像素个数     | 衡量二值化后目标占多大面积   |
| 非零占比     | 非零像素数 / 总像素数 | 归一化，不同尺寸图片之间可比 |
| 像素均值     | 所有像素的平均值      | 判断整体偏亮还是偏暗         |
| 模糊前后差值 | 模糊后前景数 − 模糊前 | 直观体现高斯模糊抑制了噪点   |





### 十、threshold 函数（Otsu算法二值化）

**threshold 的完整签名：**

`double threshold(InputArray src, OutputArray dst, double thresh, double maxval, int type);`

| 参数     | 你的值                        | 含义                                                         |
| -------- | ----------------------------- | ------------------------------------------------------------ |
| `src`    | `gray`                        | 输入必须是 **单通道 8 位灰度图** （这里前面已经 `cvtColor` 好了） |
| `dst`    | `binary`                      | 输出图，函数内部自动分配尺寸和类型                           |
| `thresh` | `0`                           | 阈值。 **配合 `THRESH_OTSU` 时这个值会被忽略** ，写 0 是惯例 |
| `maxval` | `255`                         | 满足条件的像素被赋的值（`THRESH_BINARY` 里就是白色 255）     |
| `type`   | `THRESH_BINARY | THRESH_OTSU` | 前部分是二值化方式，后部分是“自动求阈值”标志                 |

**THRESH_BINARY | THRESH_OTSU 是怎么组合的**


这两个是 按位或 的意义拼在一起的独立标志位：



THRESH_BINARY ：最基础的二值化规则

![image-20261114195708700](C:\Users\xie90\AppData\Roaming\Typora\typora-user-images\image-20261114195708700.png)


即“比阈值亮的变白，比阈值暗的变黑”。



THRESH_OTSU ：告诉函数“ 别用我给的 0，你自己算出最佳 T ”。



单独写 THRESH_BINARY 就得手动给 thresh，比如 threshold(gray, binary, 128, 255, THRESH_BINARY)，阈值固定 128，换个图效果可能很差。加上 THRESH_OTSU 后阈值自适应，thresh 位置填什么都无所谓。

**Otsu 算法在原理上做了什么**


它遍历所有可能阈值（0~255），对每个阈值把灰度直方图分成“前景”和“背景”两类，计算两类之间的 类间方差 ，取使方差最大的那个阈值。

直观理解： 让前景和背景各自的灰度尽量集中、两者差距尽量大 ，此时的阈值就是分离效果最好的点。所以它本质是“从直方图里自动找分界点”，对双峰直方图（背景一峰、目标一峰）效果尤其好。



**返回值为什么是 double**


double otsuThresh = ...



threshold 的返回值是 实际使用的阈值 ：

用 THRESH_OTSU 时 → 返回算法算出来的那个阈值（比如 118）


不用 Otsu 时 → 原样返回你传进去的 thresh



所以 otsuThresh 拿到的就是自动求出的 118，打印出来便于记录和复现：`Otsu 自动阈值 = 118`

**两个容易踩的坑**

THRESH_OTSU 只支持 8 位单通道图 。如果传彩色图或 16 位图，会直接断言失败。这就是为什么必须先 cvtColor 成灰度。


THRESH_BINARY 和 THRESH_BINARY_INV 结果相反 。如果你想让目标变黑、背景变白，可以把 type 改成 THRESH_BINARY_INV | THRESH_OTSU，之后统计“非零像素”含义就反过来了。





### 十一、morphologyEx 函数

cv::morphologyEx 是 通用形态学变换函数 ，把 erode、dilate 组合起来，一步完成开运算、闭运算、梯度、顶帽、黑帽等高级操作。

它的价值在于： 这些运算本质上都是 erode 和 dilate 的组合，OpenCV 把它们封装好，你不用手动写两遍、也不用担心中间结果 。



**和单独调用 erode/dilate 的区别：**

erode / dilate：单一操作，会改变目标尺寸


morphologyEx：组合操作，能在 保持目标尺寸基本不变 的前提下完成去噪、填充、提边缘等任务

**函数原型**

`void cv::morphologyEx(`
    `InputArray      src,`
    `OutputArray     dst,`
    `int             op,`
    `InputArray      kernel,`
    `Point           anchor = Point(-1,-1),`
    `int             iterations = 1,`
    `int             borderType = BORDER_CONSTANT,`
    `const Scalar&   borderValue = morphologyDefaultBorderValue()`
`);`

**参数说明**

| 参数          | 作用               | 注意点                                                       |
| ------------- | ------------------ | ------------------------------------------------------------ |
| `src`         | 输入图像           | 多通道逐通道独立处理；建议 8U                                |
| `dst`         | 输出图像           | 尺寸类型同 `src`，可与 `src` 相同                            |
| **`op`**      | **形态学操作类型** | 核心参数，见下表                                             |
| `kernel`      | 结构元素           | 传 `Mat()` 默认 3×3 矩形；用 `getStructuringElement` 自定义形状尺寸 |
| `anchor`      | 核锚点             | 默认 `Point(-1,-1)` 取核中心                                 |
| `iterations`  | 迭代次数           | 默认 1。开/闭运算迭代 N 次 ≈ 核放大 N 倍                     |
| `borderType`  | 边界填充           | 默认 `BORDER_CONSTANT`                                       |
| `borderValue` | 边界常数值         | 默认值对膨胀取极小、腐蚀取极大，避免边缘伪操作               |

**op 参数：六种操作**

| op 值             | 组合方式         | 效果                 | 典型用途               |
| ----------------- | ---------------- | -------------------- | ---------------------- |
| `MORPH_ERODE`     | erode            | 亮区收缩             | 等价于单独 erode       |
| `MORPH_DILATE`    | dilate           | 亮区扩张             | 等价于单独 dilate      |
| **`MORPH_OPEN`**  | erode → dilate   | 去小亮斑、断开细连线 | **去噪、分离粘连目标** |
| **`MORPH_CLOSE`** | dilate → erode   | 填小孔、接裂缝       | **闭合轮廓、填充孔洞** |
| `MORPH_GRADIENT`  | dilate − erode   | 得到边缘轮廓         | 提取目标外轮廓         |
| `MORPH_TOPHAT`    | src − open(src)  | 提亮的小细节         | 提取比核小的亮斑       |
| `MORPH_BLACKHAT`  | close(src) − src | 提暗的小细节         | 提取比核小的暗斑       |

**记忆要点 ：**

开运算 = 先腐蚀后膨胀 ，先削掉小亮物再撑回来，所以"小的没了，大的尺寸不变"


闭运算 = 先膨胀后腐蚀 ，先撑大填孔再收回来，所以"孔没了，大的尺寸不变"


单独 erode/dilate 会改变目标大小， 要"办事但不变胖瘦"就必须用开/闭运算



**三个实战注意点**


1. 输入最好是二值图。  虽然支持灰度，但形态学"取最大/最小"的语义在二值图上最清晰，灰度和彩色图的效果不易预测。


2. 核尺寸控制强度，别靠堆 iterations。  想要更强效果，先试着把 Size(3,3) 改成 Size(5,5)，比 iterations=5 更直观、更好控制。迭代次数过多会让边缘变形累积。


3. 顶帽/黑帽要求核比目标大。  MORPH_TOPHAT 提取的是"比核小的亮细节"，如果核比目标还小，什么都提不出来。这两者常用于光照不均匀校正（顶帽可用来纠正背景渐变）。



**一句话总结**


morphologyEx 是 erode/dilate 的组合封装， op 参数决定做哪一种形态学运算 ，其中开运算去小噪点、闭运算填小孔、梯度提边缘最常用；要"改变形态但不改变目标尺寸"，就别用裸 erode/dilate，直接上 morphologyEx。



**常用代码**

`Mat src = imread("test.jpg", IMREAD_GRAYSCALE);`
`Mat dst;`

`Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5,5));`

`// 开运算：去小白点噪声`
`morphologyEx(src, dst, cv::MORPH_OPEN, kernel);`

`// 闭运算：填小孔洞`
`morphologyEx(src, dst, cv::MORPH_CLOSE, kernel);`

`// 形态学梯度：取边缘`
`morphologyEx(src, dst, cv::MORPH_GRADIENT, kernel);`

`// 迭代 2 次`
`morphologyEx(src, dst, cv::MORPH_CLOSE, kernel, cv::Point(-1,-1), 2);`





### 十二、 getStructuringElement 函数

cv::getStructuringElement 是 结构元素生成器 ，它返回一个指定形状和尺寸的 Mat，专门给形态学运算当"探针"用。


结构元素（structuring element） 就是那个在图像上滑动的模板，它的形状和尺寸直接决定了：



膨胀/腐蚀 往哪个方向 扩张或收缩


影响范围 有多大


结果轮廓的 形状特征 （方角还是圆润）



它本身不做任何图像运算，只是 造核 。造好后传给 erode、dilate、morphologyEx、filter2D 等使用。

**函数原型**

`Mat cv::getStructuringElement(`
    `int     shape,`
    `Size    ksize,`
    `Point   anchor = Point(-1,-1)`
`);`

**参数说明**

| 参数        | 作用                            | 注意点                                                       |
| ----------- | ------------------------------- | ------------------------------------------------------------ |
| **`shape`** | **结构元素形状**                | 三选一：`MORPH_RECT` / `MORPH_ELLIPSE` / `MORPH_CROSS`       |
| **`ksize`** | **尺寸**  `Size(width, height)` | 决定影响范围大小；尺寸越大效果越强。宽高一般取奇数           |
| `anchor`    | 锚点位置                        | 默认 `Point(-1,-1)` 表示取核 **中心** 。只有想让形态学偏向某方向时才改 |

**结构元素的形状选择**

`getStructuringElement(cv::MORPH_RECT,    cv::Size(5,5));   // 矩形`
`getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5));   // 椭圆，轮廓更圆润`
`getStructuringElement(cv::MORPH_CROSS,   cv::Size(5,5));   // 十字，只处理横竖`

| 形状                | 生成的核示例（5×5） | 效果特征                      | 适用场景                                |
| ------------------- | ------------------- | ----------------------------- | --------------------------------------- |
| **`MORPH_RECT`**    | 全 1 的方块         | 各方向均匀扩张，结果带方角    | 默认最常用、通用去噪、整块填充          |
| **`MORPH_ELLIPSE`** | 近似圆              | 扩张/收缩更圆润、各向相对均衡 | 处理圆形目标、避免方角、轮廓平滑        |
| **`MORPH_CROSS`**   | 只有横竖中线        | 只沿水平/垂直方向作用         | 处理表格线、文字横竖笔画、水平/垂直断线 |



**返回值**


返回一个 CV_8U 单通道 Mat，里面是 0 和 1（或 0 和 255，取决于实现），形状即你所选。



**效果差异要看清 ：**

**矩形核**在 对角线方向 的作用范围比水平/垂直更远，所以膨胀后目标的直角会变"方"

**椭圆核**在对角方向作用弱，膨胀后圆目标仍是圆

**十字核**对角方向几乎不作用，所以斜向的缝隙修不了



**ksize 的作用**


尺寸越大，影响范围越大，效果越强。  这是控制强度的首选方式（比堆 iterations 更直观）。

`cv::Size(3,3);   // 影响半径约 1 像素`
`cv::Size(5,5);   // 影响半径约 2 像素`
`cv::Size(7,7);   // 影响半径约 3 像素`

为什么建议奇数 ：奇数尺寸才有唯一中心点，锚点 Point(-1,-1) 才能落到中心。偶数尺寸也能用（OpenCV 会自动取 anchor = ksize/2），但中心和核不对称，形态学结果容易偏移一两像素，调试时很难察觉。


特殊值 ：Size(0,0) 或空 Size() 在部分场景下表示"用 3×3 默认核"，但 在 getStructuringElement 里不要这么写 ，直接给明确的尺寸更稳妥。



**anchor 的基本用法**

`cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5), cv::Point(-1,-1));  // 默认中心`

只有当你想让形态学"只往一个方向扩"时才改，比如：

`// 锚点设在左中，膨胀偏向右侧`
`cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5), cv::Point(0, 2));`

日常几乎不用改 ，保持默认即可。



**一句话总结**


getStructuringElement(shape, ksize) 就是 造形态学核的工厂 ：shape 选形状（矩形/椭圆/十字），ksize 选尺寸（建议奇数，决定作用范围），anchor 基本保持默认；造好的核交给 erode/dilate/morphologyEx 用。 形状决定"怎么长"，尺寸决定"长多少" 。





### 十三、findContours 函数

cv::findContours 是 轮廓提取 函数，把二值图里每一个连通的白色区域（前景）的 边界点序列 找出来，输出成点集。

它的输出不是图像，而是 std::vector<std::vector<cv::Point>>—— 每个轮廓是一条首尾相连的点序列 。有了这条点序列，你就能算面积、周长、外接矩形、做形状匹配、判断凸凹。



**典型用途：**

目标计数 ：数连通区域（细胞、零件、颗粒）


测几何量 ：面积、周长、外接框、中心点


形状分析 ：多边形逼近、判别圆形/矩形


分割掩膜 ：把轮廓填成 mask


目标定位 ：找到最大轮廓的外接框就是主目标位置



**函数原型**

`void cv::findContours(`
    `InputArray          image,`
    `OutputArrayOfArrays contours,`
    `OutputArray         hierarchy,`
    `int                 mode,`
    `int                 method,`
    `Point               offset = Point()`
`);`

**参数说明**

| 参数         | 作用               | 注意点                                                       |
| ------------ | ------------------ | ------------------------------------------------------------ |
| `image`      | 输入二值图         | **必须 8 位单通道** （`CV_8UC1`）。OpenCV 3+ 起 **不修改输入** （4.x 无需 `.clone()`，但仍建议自己保证是干净二值图，值为 0/255） |
| `contours`   | **输出轮廓集合**   | 类型 `std::vector<std::vector<cv::Point>>`，每个元素是一条轮廓 |
| `hierarchy`  | **输出拓扑层级**   | 类型 `std::vector<cv::Vec4i>`，可选，传 `cv::noArray()` 表示不需要 |
| **`mode`**   | **轮廓检索模式**   | 决定"提取哪些层级的轮廓"，见下表                             |
| **`method`** | **轮廓点近似方法** | 决定"存多少个点"，直接影响点数多少                           |
| `offset`     | 轮廓点偏移         | 默认 `Point()`。只有当你在 ROI 子图上找轮廓、想映射回原图坐标时才用 |

**image 的硬性要求**

必须是  CV_8UC1 ，三通道直接抛异常 → 先 cvtColor 转灰度


必须是 二值 （0 和 255），否则轮廓逻辑不可预测 → 先 threshold 或 Canny

Canny 输出可以直接用 ，因为它本身就是 8UC1 二值



**mode：检索模式**

| mode            | 含义                             | 输出特征                             |
| --------------- | -------------------------------- | ------------------------------------ |
| `RETR_EXTERNAL` | 只提取 **最外层** 轮廓           | 忽略孔洞内嵌轮廓，最常用，速度快     |
| `RETR_LIST`     | 提取 **所有** 轮廓，不建层级关系 | 所有轮廓平铺，`hierarchy` 无实际意义 |
| `RETR_CCOMP`    | 两层结构：外轮廓 + 内孔          | 每个目标的孔洞归到它的父级下         |
| `RETR_TREE`     | **完整树形结构**                 | 保留所有嵌套关系，最全但最慢         |

**选择建议 ：**

只关心目标外边界、数个数 → RETR_EXTERNAL


需要分析孔洞（如环形零件、字母 O）→ RETR_CCOMP 或 RETR_TREE


完全不关心层级、只要全部轮廓 → RETR_LIST



**method：近似方法**

| method                    | 含义                         | 点数特点                            |
| ------------------------- | ---------------------------- | ----------------------------------- |
| `CHAIN_APPROX_NONE`       | 存储 **所有** 边界点         | 点数最多，轮廓完整，可能很占内存    |
| **`CHAIN_APPROX_SIMPLE`** | 压缩 **直线段** ，只保留端点 | **默认推荐** ，点数少很多，形状不变 |
| `CHAIN_APPROX_TC89_L1`    | Teh-Chin 链近似（L1）        | 进一步压缩，略有形变                |
| `CHAIN_APPROX_TC89_KCOS`  | Teh-Chin 链近似（KCOS）      | 同上，另一种代价度量                |

CHAIN_APPROX_SIMPLE 是最常用的 ：一条水平直边原本可能有 100 个点，压缩后只留两端 2 个点，后续处理快得多，而几何量计算基本不受影响。



**五个高频坑**


1. 输入不是单通道。  传 BGR 图直接崩。cvtColor 是必经步骤。


2. 输入不是二值。  灰度图直接喂进去不报错，但结果混乱（OpenCV 内部把非零当 1 处理，边界判定会飘）。先 threshold。


3. 前景/背景反了。  findContours 找的是 白色区域 的轮廓。如果目标在图上显示为黑色，要先 bitwise_not 反色，否则找到的是背景框。


4. 面积过滤缺失。  二值图总有零星噪点，不按 contourArea 过滤，contours.size() 会虚高。area < 50~100 这段过滤几乎是标配。


5. 坐标基于当前 Mat。  如果在 ROI 上跑，坐标是 ROI 局部坐标，画到原图会整体偏移——用 offset 参数或手动 +roi.tl()。



### 十四、boundingRect 函数

cv::boundingRect 是 正外接矩形计算 函数，输入一组点（或一条轮廓），输出能 完全框住这些点的最小水平垂直矩形 （cv::Rect）。

**注意它的三个特征：**

正矩形 ：边始终平行于图像坐标轴，不旋转。想要带旋转角的框用 minAreaRect（返回 RotatedRect）


最小 ：在正矩形中，它是面积最小的那个


输入是点集，不是图像 ：它不关心像素值，只关心坐标范围



**典型用途：**

定位目标 ：findContours 之后给每个轮廓画框


裁剪 ROI ：用框去抠出目标区域做后续识别


框合并 ：多个小点围成一团时得到整体范围


快速获取尺寸 ：轮廓的宽高信息



**函数原型**

`Rect cv::boundingRect(InputArray array);`

只有一个参数 ，其他什么都没有：

| 参数    | 作用         | 注意点                                                       |
| ------- | ------------ | ------------------------------------------------------------ |
| `array` | **输入点集** | 可以是 `vector<Point>`、`vector<Point2f>`、`Mat`（N×1 或 1×N 的 2 通道点阵），也 **可以直接传一条轮廓**  `contours[i]` |

**返回值**


返回 cv::Rect，四个成员：

| 成员     | 含义                     |
| -------- | ------------------------ |
| `x`      | 外接框左上角 **列** 坐标 |
| `y`      | 外接框左上角 **行** 坐标 |
| `width`  | 宽度 = maxX - minX + 1   |
| `height` | 高度 = maxY - minY + 1   |

注意宽度是  max - min + 1 ，因为是"像素个数"而不是"坐标差"。一个只占 1 像素的点，返回 width = 1 而不是 0。

**array 的几种合法形式**

`std::vector<cv::Point>   contour;      // 最常见：直接传一条轮廓`
`std::vector<cv::Point2f> pts;          // 浮点坐标也可以`
`cv::Mat pointsMat;                     // N×1 CV_32SC2 或 CV_32FC2`

`cv::Rect r1 = cv::boundingRect(contour);`
`cv::Rect r2 = cv::boundingRect(pointsMat);`

findContours 输出的 contours[i] 直接丢进去就行，这是最典型的用法。



**和 minAreaRect 的区别**

|            | `boundingRect`             | `minAreaRect`               |
| ---------- | -------------------------- | --------------------------- |
| 返回类型   | `cv::Rect`                 | `cv::RotatedRect`           |
| 是否旋转   | **不旋转** ，轴对齐        | 可旋转任意角度              |
| 面积       | 通常偏大（旋转目标会留白） | 更贴合，最小                |
| 用途       | 画框、裁剪 ROI、快速定位   | 测倾斜角度、精确尺寸        |
| 转成普通框 | 本身即是                   | `boundingRect(rotatedRect)` |

判断依据 ：目标倾斜明显、且你要测真实宽高 → minAreaRect；只是要一个框来画图或抠图 → boundingRect 足够且更快。



**常用代码**

给每个轮廓画外接框

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    for (const auto& c : contours) {
        if (cv::contourArea(c) < 100) continue;        // 过滤噪点
    
        cv::Rect box = cv::boundingRect(c);
        cv::rectangle(src, box, cv::Scalar(0, 255, 0), 2);
    }
**几点实战提醒**


1. 坐标是当前图像的坐标。  如果你在 ROI 子图上找轮廓，boundingRect 得到的是局部坐标，画回原图要 + roi.tl() 或 offset 平移。


2. 它只框点，不框像素外扩。  有时候想让框稍微外扩一点再裁剪：

```
cv::Rect box = cv::boundingRect(c);
int pad = 5;
box = cv::Rect(box.x - pad, box.y - pad, box.width + 2*pad, box.height + 2*pad);
box &= cv::Rect(0, 0, src.cols, src.rows);
```

3. 点数很多时它很快。  它只扫一遍求 min/max，复杂度线性，性能几乎不用担心，轮廓几千个也能直接循环。



**一句话总结**


boundingRect(点集或轮廓) 只需要 一个参数 ——一组点，返回 轴对齐的最小正外接矩形  Rect；它不旋转、不算像素值，只求坐标范围，所以通用性最强、速度最快，但目标倾斜时框会偏大——需要量真实尺寸或角度就换 minAreaRect。



### 十五、for循环中 const auto& c : contours 是什么

它等价于传统的下标循环：

```
for (size_t i = 0; i < contours.size(); ++i) {
    const auto& c = contours[i];   // 等价
    cv::Rect box = cv::boundingRect(c);
}
```

但省掉了下标、size() 和越界风险，写起来干净很多。



1. contours —— 被遍历的容器


findContours 输出的类型是：

```
std::vector<std::vector<cv::Point>> contours;
```

两层 vector 。所以：

容器里装了很多元素，每个元素是 std::vector<cv::Point>（ 一条轮廓 ）


循环每次拿出的 c 就是 其中一条轮廓 ，类型是 std::vector<cv::Point>

所以 c 直接就能传给 boundingRect(c)，因为它正是一条轮廓。