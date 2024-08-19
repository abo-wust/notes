


**NUD\_NONE：**The neighbor entry has just been created and no state is available yet.

**NUD\_INCOMPLETE：**A solicitation has been sent, but no reply has been received yet. In this state, there is no hardware address to use (not even an old one, as there is with NUD\_STALE).

**NUD\_REACHABLE：**The address of the neighbor is cached（ARP response is received） and the latter is known to be reachable (there has been a proof of reachability).

**NUD\_FAILED：**Marks a neighbor as unreachable because of a failed solicitation request, either the one generated when the entry was created or the one triggered by the NUD\_PROBE state.

**NUD\_STALE：**When neigh\_periodic\_timer(), finds that the last time the entry was confirmed exceded the reachable time parameter.

**NUD\_DELAY：**When neigh\_resolve\_output() is eventually called with a struct neighbour in the NUD\_STALE state.

**NUD\_PROBE：**Transitional states; they will be resolved when the local host determines whether the neighbor is reachable.

The next set of values represents a group of special states that usually never change once assigned:

**NUD\_NOARP：**This state is used to mark neighbors that do not need any protocol to resolve the L3-to-L2 mapping.（不需要解析，如PPP接入）

**NUD\_PERMANENT：**The L2 address of the neighbor has been statically configured (i.e., with user-space commands) and therefore there is no need to use any neighboring protocol to take care of it. （静态ARP表项）


In addition to the basic states listed in the previous section, the following derived values are defined just to make the code clearer when there is a need to refer to multiple states with something in common:


**NUD\_VALID：**An entry is considered to be in the NUD\_VALID state if its state is any one of the following, which represent neighbors believed to have an available address:

  NUD\_PERMANENT
  NUD\_NOARP
  NUD\_REACHABLE
  NUD\_PROBE
  NUD\_STALE
  NUD\_DELAY

  


**NUD\_CONNECTED：**This is used for the subset of NUD\_VALID states that do not have a confirmation process pending:

  NUD\_PERMANENT
  NUD\_NOARP
  NUD\_REACHABLE




**NUD\_IN\_TIMER：**The neighboring subsystem is running a timer for this entry, which happens when the status is unclear. The basic states that correspond to this are:

  NUD\_INCOMPLETE
  NUD\_DELAY
  NUD\_PROBE




1. 状态机迁移简介

当网络层发送报文前首先需要查找路由，出口路由是和邻居绑定的。路由查找完成后会调用邻居层提供的output接口发送。发送函数output会随着邻居项的状态改变。邻居项的状态？当然啦，邻居项也是有状态的，比如说刚建立邻居项的时候，这时候还不知道邻居的MAC地址，这个邻居项还不能使用，因为是初始化，所以状态时NONE，这个时候如果发送报文的话时没办法发送出去的，必须先使用邻居协议发送solicit请求，这个时候邻居项的状态就会变成INCOMPLETE(未完成)，创建邻居项的时候会自动起一个定时器，当定时器超时的时候会检查当前邻居项的状态并作出适当改变。当发送solict请求一段时间没有响应回来的话定时器就会超时，这时候会根据当前状态判断是否需要重传，重传的次数一定的，不可能一直重传下去，每次重传后定时器会自动重启，定时器超时的时间也是根据配置来的，重传的定时器时间是neigh->parms->retrans\_time。此外，在发送solicti请求期间是没法传输报文的，这个时候怎么办呢，总不能系统就停在这里吧，当然也不能丢弃报文，可能邻居一会儿就响应了。这个时候需要把这个报文放到neigh->arp\_queue缓存队列里，当然队列是有长度的，不可能无线存储，不然内存就不够了，默认是存储三个报文，溢出后简单丢弃最先进来的。队列长度是可配的。

假设收到了响应，这时候邻居状态就会从INCOMPLETE状态迁移到REACHABLE(可到达)，这个时候邻居是可到达的，除了迁移状态外还需要把缓存队列里面的报文发送出去。

当然状态不可能一直是Reachable(可到达)，可能邻居down掉了，或者我们设备自己挂掉了，这个时候邻居状态必须更改。通常情况下，如果一段时间不用，邻居状态就会从reachable状态迁移到stale（旧）状态，这个时候需要可到达性确认了。

如果在gc\_staletime没有使用的话状态就会迁移到fail，此时gc定时回收。如果在gc\_staletime有使用的话，状态迁移到delay状态，相当于延迟迁移到fail状态，在delay状态经过delay\_probe\_time状态没有更新的话就会进入probe状态，这个状态下需要主动发送探测报文，发送探测报文的次数是有限的，超时的话就只能丢弃了。

邻居状态迁移图如下：

![800](images/Pasted%20image%2020221201232325.png)

上图为中文版附图，存在几处翻译错误，对照下图英文版附图理解。

- REACHABLE向STALE状态迁移时，应该理解为超过配置的REACHABLE\_TIME时间该表项未使用，例如配置该时间为100秒，则超过100秒，若该表项未使用，则状态迁移到STALE状态。
- DELAY向REACHABLE状态迁移时，应该理解为delay定时器超时前收到ARP应答或者L4确认。
- STALE状态自检时，应该理解为在gc\_staletime定时器未超时的时间内，该表项被使用，所以gc\_staletime定时器会重置，保持STALE状态。
- 而当gc\_staletime定时器超时，该表项未被使用时，状态机会从STALE迁移到FAILED，并删除该表项（看内核代码是neigh\_delete更新状态并删除表项）。



![](images/Pasted%20image%2020221201232331.png)














