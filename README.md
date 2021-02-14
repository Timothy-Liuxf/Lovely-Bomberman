# Lovely-Bomberman

---

程设大作业之可爱炸弹人

素材来源：

1. [4399可爱炸弹人](http://www.4399.com/flash/17742_3.htm)  

2. Minecraft (Mojang)  

**声明：本软件仅限于计算机程序设计基础(2)夏季学期作业用，不作商业用途**

---

编译说明：采用 MSVC v142 进行编译，语言标准 `/std:c++17`。

将所有项目编译完成后，运行 `ReleaseProduct.bat` 导出 32 位程序，或运行 `ReleaseProduct(x64).bat` 导出64位程序。导出的程序相应地存储在 `可爱炸弹人_Product` 或 `可爱炸弹人_Product(x64)` 目录内。

---

这是本人第一次写这么大的工程，也是第一次尝试多线程，所以首先思路不是很清晰，技术不是很成熟，另外也有很多设计不合理的地方，也没有很大胆地开太多线程。一些不足之处，至少是个人感觉需要大幅改善的：

1. 滥用了`dynamic_cast`，里面所有的`dynamic_cast`都应该换成`static_cast`，这是我当时对`dynamic_cast`的误解造成的
2. 没有精确控制游戏帧数，导致帧数不稳，依赖于电脑的执行速度，强烈建议自行获取时间戳计时稳定帧数
3. 所有游戏对象数据的更新都在同一个线程中，效率过低，此处应分别开线程
4. 游戏逻辑和界面的代码界限不清晰，游戏逻辑的一部分代码写到了界面代码中；导致的结果就是人物被炸后的保护时间的设置存在BUG，当游戏结束时需要等待保护时间结束才能退出。

不足之处，还请谅解。希望有兴趣的人可以将上面这些问题修复（虽然目测是一个极其庞大的工程），不胜感激。

