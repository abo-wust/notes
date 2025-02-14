![](images/附图.drawio)



一种多CPU协同工作的方法和设备



**摘要**

  本发明涉及一种多CPU协同工作的方法和设备，该方法和设备包括：使用四片相同型号的CPU分别运行相同版本的应用程序；四片CPU串联由硬件连线区分第一片为主CPU，其余三片为从CPU；主CPU通过SPI控制或监测从CPU的功能和状态；主CPU的SPI初始化为主工作模式，从CPU的SPI初始化为从工作模式；上层应用程序配置业务时，不区分CPU主从，但是主所承担的工作内容较从更多，主CPU需要通过SPI对从CPU的功能或状态进行配置和监测；四片CPU由FPGA区分各自编号以方便使用。




**权利要求书**

  1\. 一种多CPU协同工作的方法和设备，其特征在于，包括：

  设备启动时由应用程序初始化所有驱动；

  通过读取指定PHY地址数据区分主从CPU角色；

  主从CPU分别初始化SPI工作模式，确保主从通信连接畅通；

  主从CPU各自加载FPGA，并初始化相关配置；

  应用程序下发业务配置，主CPU监测包括自己在内的所有四片CPU工作状态，并对相应状态进行处理；

  设备的整体操作均由主CPU进行处理，从CPU负责提供状态和处理主的控制消息。

  2\. 根据权利要求1所述的一种多CPU协同工作的方法和设备，其特征在于，还包括：

  主从CPU的SPI都映射到用户空间，在用户空间实现SPI的从工作模式。

  3\. 根据权利要求1所述的一种多CPU协同工作的方法和设备，其特征在于，还包括：

  应用程序启动时，主CPU需要先加载PLL，然后通过I2C获取PLL时钟状态，当PLL处于锁定状态之后再加载FPGA，同时通过SPI通知从CPU时钟已经稳定。


4\. 根据权利要求3所述的一种多CPU协同工作的方法和设备，其特征在于，还包括：

  主从CPU与PLL之间都有硬件连线来获取时钟，但是只有主CPU能读取PLL时钟状态。

  5\. 根据权利要求3和权利要求4所述的一种多CPU协同工作的方法和设备，其特征在于，还包括：

  从CPU初始化完成SPI的从工作模式，使用单独的线程监听SPI的消息，加载FPGA前应用程序保持阻塞等待，直到确定时钟处于锁定状态。从SPI收到主CPU的PLL时钟锁定消息后，从CPU解除等待状态，应用程序开始加载FPGA。

  6\. 根据权利要求1所述的一种多CPU协同工作的方法和设备，其特征在于，还包括：

  主从CPU的业务都需要保证SERDES接口的正常工作，而SERDES接口的异常都需要主CPU统一处理，避免单独处理导致的业务问题。主CPU通过SPI周期性读取本地以及三片从CPU的SERDES接口状态，当从CPU的SERDES接口异常时，主CPU通过SPI发送复位消息，从CPU收到该复位消息后执行复位操作。






**技术领域**

  本发明属于通信技术领域，具体涉及一种多CPU协同工作的方法和设备。




**背景技术**

在硬件电路的设计中，为简化设计和系统结构，常用一组线路，配置相应的接口电路，用于连接各个硬件部件和外围设备，这组共用的线路称为总线。总线就是计算机各种功能部件之间传送信息，进行通信的公共通路。总线按功能和规范可分为：数据总线、地址总线、控制总线、扩展总线及局部总线等五种类型。按传输数据的方式则可以分为串行总线和并行总线。按照时钟信号是否独立则可以分为同步总线和异步总线。

微机系统中一般有内部总线、系统总线和外部总线。在硬件设计中，经常涉及各个功能芯片间的连线，所以内部总线，如：I2C、SPI、UART、JTAG、CAN等较为常见。不同的总线适用于不同的场景，而实际应用中也会根据需要选择合适的总线方案。

在SPI总线的应用中，CPU一般作为主模式使用，其他外围设备作为从来响应控制CPU的读写。但是，实际的设计应用总会出现各种情况，使得其他总线方案不可用。比如本发明中涉及到的多CPU协同工作的场景，由于硬件方案和资源问题，需要一个CPU作为主控制器，其他三片CPU作为从机来实现整机设备的业务配置可用。这就需要将SPI在CPU上作为从工作模式使用，并且为了软件的灵活性，本发明还将SPI映射到用户空间，实现功能可定制。一主三从的多CPU协同工作，在用户空间的SPI主从工作模式下能得到很好的处理。





**发明内容**

为解决在特殊情况下多CPU协同工作只能使用SPI方式进行通信的问题，本发明提供一种多CPU协同工作的方法和设备，该方法包括：

  步骤1，主从CPU启动时，通过读取指定PHY地址，确定本CPU的主从角色；

  步骤2，主从CPU初始化SPI，主CPU获取三片从CPU的SPI接口地址，从CPU将SPI映射到用户空间，并在用户空间实现SPI消息的收发处理逻辑；

  步骤3，主CPU加载PLL，从CPU得到PLL锁定前保持等待，主CPU通过I2C读取PLL状态；

  步骤4，主CPU读取到PLL状态锁定后，向从CPU发送SPI消息，通知PLL已经锁定，从CPU收到消息后，应用程序解除等待状态，开始加载FPGA，完成应用程序后续配置；

  步骤5，主CPU通过SPI读取从CPU的FPGA是否加载完成，三片CPU的FPGA都加载成功后，主CPU开始加载FPGA，完成应用程序后续配置；

  步骤6，应用程序正常启动后，主CPU使用单独的线程通过SPI配置或监测从CPU的功能或状态；

  步骤7，用户空间实现的SPI读操作出现异常时，主CPU通过SPI的写操作复位从CPU的SPI处理逻辑，以保证SPI读写通道的正常，避免主从CPU的协同工作。






**附图说明**

  图1所示为本发明的设备模型结构图。






**具体实施方式**

针对主从角色的获取，四片CPU使用同一版本的应用程序，在上电启动时，软件无法区分应用程序所在的环境是主还是从CPU。通过硬件给定的PHY寄存器地址，应用读取该地址时得到的值不同来区分主从CPU，并将是否主CPU作为全局常量g\_is\_master保存。

应用程序得到本CPU的工作角色后，根据主从分别对SPI进行初始化。设备在应用程序启动前的上电过程中加载设备树，得到三片从CPU的SPI设备树对象。

  主CPU的初始化打开三个从CPU的SPI文件描述符，并保存。

  从CPU初始化将SPI的地址通过内存映射到用户空间，创建线程监测SPI的中断状态寄存器以及接收队列是否为空，将收到的消息从队列中取出。创建另一个线程处理从队列中取出的SPI消息。同时，配置根据芯片手册初始化配置、中断和使能相关的SPI寄存器，确保用户空间的线程能读取到正确的中断信号，从收队列能正常取出数据。

  主CPU初始化SPI之后开始加载PLL，而硬件条件使得从CPU的应用程序无法获取PLL状态，因此无法继续加载FPGA，进入等待状态。主CPU执行PLL的加载后进入等待，并通过I2C开始读取PLL的状态，当PLL锁定之后，主CPU发送PLL锁定的SPI消息到从CPU，然后主CPU开始SPI读取从CPU的FPGA是否加载成功。

  从CPU收到PLL锁定的状态后，解除等待状态，开始加载FPGA，加载成功之后，将g\_slave\_fpga\_ready标志置位，并继续应用程序的后续配置。主CPU通过SPI读取到三片从CPU的g\_slave\_fpga\_ready都置位之后，开始加载主的FPGA，并继续应用程序的后续配置。

  主从的应用程序成功运行后，从CPU上的应用程序负责业务的下发、业务接口的配置和状态监测、内部接口的监测和动作等，而主CPU的应用程序除这些内容外，还需要监测PLL的状态并通知从、从CPU内部接口的监测和动作，以及其他需要协同的功能或状态等工作。

  从CPU上的SPI消息接收线程10ms周期性读取SPI的中断状态寄存器，如果有中断消息，则判断是否是接收队列有数据。如果接收队列有数据，则读取数据接收寄存器SPI\_RXD到应用程序的缓存数组。SPI能接收的最大数据长度为128字节，除四个控制字节外，可供使用的数据长度为124字节。四个控制字节分别为：command字段标识读写动作，define字段用于自定义消息，以及两个addr字节标识访问的寄存器地址。

  消息接收线程将数据取出后交由消息处理线程处理，消息处理线程根据四个控制字节判断读写动作，以及读写的地址。如果是SPI读消息，则从CPU将本地的数据按规则填入SPI的缓存数组，然后将该数组通过SPI的数据发送寄存器SPI\_TXD返回给主CPU。注意，从CPU在应答SPI的读消息时，填入缓存数组的前四个字节需要空置，有用数据从第五个字节开始填入。如果是SPI写消息，根据控制字节的define字段和addr地址字段，调用对应的函数来处理主CPU发送的写命令。

  主CPU在发送SPI读消息时，填入四个控制字节后，将要发送的数据发送两次，第一次发送目的是通知从CPU的消息接收线程有消息到达，需要准备好相应的数据通过SPI\_TXD将数据返回，第二次间隔100ms发出，是主CPU将从CPU应答的数据从SPI队列中取出。

  用户空间实现的SPI读写消息需要保证稳定性，因此，主CPU需要对SPI消息的收发进行监测。主CPU如果连续监测到SPI的读消息返回数据都是异常，比如全0或全F，则需要对应的从CPU的SPI处理线程进行复位操作。本发明在主CPU周期性1秒一次读取从CPU的某个状态时进行判断，如果连续三个读取周期，即3秒内读取的数据三次都为0，则发送SPI写命令，对对应的从CPU的SPI处理线程进行复位重置。具体判断条件根据实际使用设定。该复位操作仅允许主CPU执行，从CPU无法自行复位，避免主从协同出现不可预知问题。






**附图**



![](images/Pasted%20image%2020221202002340.png)



图1



