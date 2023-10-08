
1，四川移动智能光收主要使用有两种设备，一种是HOFL-11000S4N/1101S4N系列    另外一种是 H18EDD-0402B.V2
2，传输设备都是华为系列，支持掉电告警

问题：
1，两类光收的EFM功能打开以后，临终遗言都能正常上报，但是传输网管上会收到ETH_EFM_REMFAULT告警，不在省公司考核范围内
2，HOFL-11000S4N/1101S4N系列设备只要网口link起来，传输网管上ETH_EFM_REMFAULT告警会消失，但是H18EDD-0402B.V2设备网口link，告警不会消失
3，H18EDD-0402B.V2设备和1100系列机制不一样吗？设备网口link起来后，ETH_EFM_REMFAULT告警依然无法消除。


简述：

问题（1）：
电口link影响光口的efm，这个是定制需求，电口的告警会放到efm协议中

问题（2）：
就是0402B设备的电口link后，对端传输设备上的ETH_EFM_REMFAULT告警应该消失
但是，现在问题就是即使电口link，告警依然存在


定位：

1. 消除ETH_EFM_REMFAULT告警
    
    华为网管上报的ETH_EFM_REMFAULT告警是efm的critical事件，我们设备上只有低温和高温告警会触发efm的critical事件告警，把设备的高温告警阈值设高一点即可


2. 电口link关联到efm
    
    这个需求属于功能开发。
    
    电口link关联到efm后，原先高温和低温告警触发efm的逻辑需要删除，否则无法通过efm的critical事件定位具体告警。
    






