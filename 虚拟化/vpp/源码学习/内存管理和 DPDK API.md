
https://mp.weixin.qq.com/s/9Y7Ey55fIUvIlop9r_QMGw
https://www.dpdk.org/memory-in-dpdk-part-1-general-concepts/

本译文源自PANTHEON.tech公司技术博客文章《VPP 105: Memory Management & DPDK APIs》，在我们深入运用VPP的过程中，不免对其中涉及的各类技术与库——它们的实践应用、优势特性及对应API——产生诸多疑问。这篇针对性的文章恰好为我们详尽答疑，扫清困惑。很适合入门vpp的同学学习。

> pantheon.tech是一家成立于2001年，总部位于斯洛伐克的首都-布拉迪斯拉发的公司。主要致力于SDN和NFV网络技术解决方案。是开源项目OpenDaylight（简称 ODL，是一个开源的软件定义网络SDN控制器平台）的主要贡献者；也是Fd.io项目中重要参与者。

## 为什么DPDK使用大页：

**大页**是虚拟内存管理系统中的一项关键技术。在常规场景下，CPU会为每个进程分配若干虚拟内存块，这些块被称作“页”，且在Linux内核中，单个页的标准尺寸为4KB。当进程尝试访问其内存时，CPU必须首先确定该虚拟内存地址对应的物理位置，这一任务正是由内存管理单元（MMU）通过查询页表来完成的。借助于页表的层级结构，CPU能够有效地将虚拟内存地址空间映射到实际的物理内存之中。

例如，当某个进程请求高达1GB的内存时，CPU将不得不在页表中遍历超过200,000个页面条目来进行寻址，这一过程无疑会对性能造成显著影响。幸运的是，现代处理器支持使用大页面技术，即增大单个页面的尺寸。通过使用大页面，不仅能够显著减少页表中的条目数量，还能有效地降低CPU在页表中查找所需页面的次数，进而提升系统性能。

内存管理系统依赖于转换后备缓冲区（Translation Lookaside Buffer, TLB）这一硬件缓存机制来加速虚拟地址到物理地址的转换。每当发生地址转换时，MMU（Memory Management Unit）负责计算并记录该映射关系于TLB中。这样一来，后续对同一页面的访问首先由快速响应的TLB处理，仅在TLB未命中时才交由MMU进行较耗时的查表操作。

鉴于TLB的缓存容量有限，大量细粒度的页面会迅速耗尽其条目，从而导致应用程序速度下降。此时，大页面与TLB的协同作用便显得尤为重要：它们共同减少了虚拟到物理页面地址的转换次数及查找时间，进一步增强了系统的整体性能。

正因如此，DPDK（Data Plane Development Kit）和VPP（Vector Packet Processing）等==**高性能网络处理框架选择采用Hugepages（巨页）来分配大规模内存池，特别是用于存放数据包缓冲区。采用大页面分配内存，不仅减少了所需的页面总数和寻址查找次数，还提升了内存管理效率，从而有力地助推了整体性能的提升**==。


**缓存预取**是 VPP 用于提高执行性能的另一项技术。在实际需要数据之前，将数据从较慢内存中的原始存储预取到较快的本地内存中，从而显著提高性能。CPU 具有快速的本地高速缓存，可在需要时保存预取的数据。具有特定功能的 CPU 高速缓存的示例包括 D 高速缓存（数据高速缓存）、I高速缓存（指令高速缓存）和 MMU 的 TLB（转换后备缓冲区）。分离的 D 高速缓存和 I 高速缓存使得并行获取指令和数据成为可能。此外，指令和数据具有不同的访问模式。

缓存预取主要用在节点处理报文时。在VPP中，每个节点都有一个注册功能，负责传入流量处理。注册示例（abf和flowprobe节点）：

```
VLIB_REGISTER_NODE (abf_ip4_node) = {  
  .function = abf_input_ip4,  
  .name = "abf-input-ip4",  
  
VLIB_REGISTER_NODE (flowprobe_ip4_node) = {  
  .function = flowprobe_ip4_node_fn,  
  .name = "flowprobe-ip4",
```

在ABF（Access Control List Based Forwarding，基于访问控制列表转发）处理函数中，我们可以观察到其采用了单一循环处理机制——即遍历所有数据包并逐一进行处理。

```
/*  
 * Copyright (c) 2017 Cisco and/or its affiliates. Licensed under the Apache License, Version 2.0. You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0  
 */  
  
abf_input_inline (vlib_main_t * vm,  
      vlib_node_runtime_t * node,  
      vlib_frame_t * frame, fib_protocol_t fproto)  
{  
...  
      while (n_left_from > 0 && n_left_to_next > 0)  
  {  
...   
    abf_next_t next0 = ABF_NEXT_DROP;  
    vlib_buffer_t *b0;  
    u32 bi0, sw_if_index0;  
...  
    bi0 = from[0];  
    to_next[0] = bi0;  
    from += 1;  
    to_next += 1;  
    n_left_from -= 1;  
    n_left_to_next -= 1;  
  
    b0 = vlib_get_buffer (vm, bi0);  
    sw_if_index0 = vnet_buffer (b0)->sw_if_index[VLIB_RX];  
  
    ASSERT (vec_len (abf_per_itf[fproto]) > sw_if_index0);  
    attachments0 = abf_per_itf[fproto][sw_if_index0];  
...  
    /* verify speculative enqueue, maybe switch current next frame */  
    vlib_validate_buffer_enqueue_x1 (vm, node, next_index,  
             to_next, n_left_to_next, bi0,  
             next0);  
  }  
      vlib_put_next_frame (vm, node, next_index, n_left_to_next);  
    }
```

在flowprobe节点中，我们观察到采用了预取技术的四重/单循环结构，这一设计能够显著提升性能表现。该结构依次处理缓冲区b0和b1（同时预先加载接下来的两个缓冲区），随后的循环则负责处理余下的数据包。

> CPU预取对软件性能的影响是双面的：在理想条件下，预取可以显著提升内存访问效率，减少延迟，优化程序运行速度；但在预测不准或不适合预取的场景下，它可能引入额外的开销，降低缓存利用率，反而影响性能。合理利用预取技术，包括硬件自动预取和软件指导预取，以及考虑其与分支预测等其他性能优化技术的相互作用，是提升特定应用性能的关键。程序员应根据程序特性和性能分析结果，审慎决定是否以及如何利用预取技术进行优化。

```
/*  
 * Copyright (c) 2018 Cisco and/or its affiliates. Licensed under the Apache License, Version 2.0. You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0  
 */  
  
flowprobe_node_fn (vlib_main_t * vm,  
       vlib_node_runtime_t * node, vlib_frame_t * frame,  
       flowprobe_variant_t which)  
{  
...  
      /*  
      * While we have at least 4 vector elements (pkts) to process..  
      */  
  
      while (n_left_from >= 4 && n_left_to_next >= 2)  
  {  
...  
    /* Prefetch next iteration. */  
    {  
      vlib_buffer_t *p2, *p3;  
  
      p2 = vlib_get_buffer (vm, from[2]);  
      p3 = vlib_get_buffer (vm, from[3]);  
  
      vlib_prefetch_buffer_header (p2, LOAD);  
      vlib_prefetch_buffer_header (p3, LOAD);  
  
      CLIB_PREFETCH (p2->data, CLIB_CACHE_LINE_BYTES, STORE);  
      CLIB_PREFETCH (p3->data, CLIB_CACHE_LINE_BYTES, STORE);  
    }  
...  
    /* speculatively enqueue b0 and b1 to the current next frame */  
    b0 = vlib_get_buffer (vm, bi0);  
    b1 = vlib_get_buffer (vm, bi1);  
  
  
    /* verify speculative enqueues, maybe switch current next frame */  
    vlib_validate_buffer_enqueue_x2 (vm, node, next_index,  
             to_next, n_left_to_next,  
             bi0, bi1, next0, next1);  
  }  
      /*  
      * Clean up 0...3 remaining packets at the end of the frame  
      */  
      while (n_left_from > 0 && n_left_to_next > 0)  
  {  
    u32 bi0;  
    vlib_buffer_t *b0;  
    u32 next0 = FLOWPROBE_NEXT_DROP;  
    u16 len0;  
  
    /* speculatively enqueue b0 to the current next frame */  
    bi0 = from[0];  
    to_next[0] = bi0;  
    from += 1;  
    to_next += 1;  
    n_left_from -= 1;  
    n_left_to_next -= 1;  
  
    b0 = vlib_get_buffer (vm, bi0);  
  
    vnet_feature_next (&next0, b0);  
  
    len0 = vlib_buffer_length_in_chain (vm, b0);  
    ethernet_header_t *eh0 = vlib_buffer_get_current (b0);  
    u16 ethertype0 = clib_net_to_host_u16 (eh0->type);  
  
    if (PREDICT_TRUE ((b0->flags & VNET_BUFFER_F_FLOW_REPORT) == 0))  
      {  
        flowprobe_trace_t *t = 0;  
        if (PREDICT_FALSE ((node->flags & VLIB_NODE_FLAG_TRACE)  
         && (b0->flags & VLIB_BUFFER_IS_TRACED)))  
    t = vlib_add_trace (vm, node, b0, sizeof (*t));  
  
        add_to_flow_record_state (vm, node, fm, b0, timestamp, len0,  
          flowprobe_get_variant  
          (which, fm->context[which].flags,  
           ethertype0), t);  
      }  
  
    /* verify speculative enqueue, maybe switch current next frame */  
    vlib_validate_buffer_enqueue_x1 (vm, node, next_index,  
             to_next, n_left_to_next,  
             bi0, next0);  
  }
```


## 为什么轮询比中断更快？硬件/软件中断是如何工作的？

在VPP中，I/O设备（如网卡）事件处理是至关重要的部分。CPU并不知晓I/O事件何时会发生，但必须做出响应。针对这一需求，有两种截然不同的方法——中断（IRQ）和轮询（Polling），两者在多个方面存在差异。

从CPU的角度来看，中断似乎更具优势，因为在这种模式下，设备仅在需要服务时才打扰CPU，而不像轮询那样持续不断地检查设备状态。然而，从效率角度看，当设备频繁地反复中断CPU时，中断方式效率低下；而当CPU大部分时间都在等待设备准备就绪时，轮询则显得低效。

对于VPP中的包处理场景，通常预期网络流量将持续不断。在此情况下，中断次数会急剧增加。相反，设备几乎始终处于待服务状态。因此，对于包处理而言，轮询方式更为高效。这也是VPP在处理入站包时选择采用轮询机制的原因。


## DPDK提供了哪些API？VPP如何使用这个库？

DPDK网络驱动分为两大类：1. 实体设备驱动；2. 模拟设备驱动

DPDK的ethdev层对外提供了API接口，以供调用这些设备的网络功能。欲了解支持的所有特性和API详细列表，请阅读DPDK官方文档。

在VPP中，为了简化DPDK接口的启用/禁用与管理，DPDK支持已从核心部分迁移到插件形式。为方便存储和管理所有与DPDK相关的信息，VPP在源代码（src/plugin/dpdk/device/dpdk.h）中实现了DPDK设备结构，其中包含DPDK数据：

```
/* SPDX-License-Identifier: BSD-3-Clause  
 * Copyright(c) 2010-2014 Intel Corporation  
 */  
  
typedef struct  
{  
...  
  struct rte_eth_conf port_conf;  
  struct rte_eth_txconf tx_conf;  
...  
  struct rte_flow_error last_flow_error;  
...  
  struct rte_eth_link link;  
...  
  struct rte_eth_stats stats;  
  struct rte_eth_stats last_stats;  
  struct rte_eth_xstat *xstats;  
...  
} dpdk_device_t;
```

包含所有在VPP中使用的相关DPDK结构体，用于存储与DPDK相关的信息。

DPDK API仅在DPDK插件中使用。以下是VPP中使用的一系列DPDK特性及其API列表，附带一些使用示例。

**速率能力 / 运行时接收/发送队列设置** 支持获取当前设备所支持的速率能力。支持设备启动后进行接收队列设置。API: rte_eth_dev_info_get()

```
/*  
 * Copyright (c) 2017 Cisco and/or its affiliates. Licensed under the Apache License, Version 2.0. You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0  
 */

dpdk_device_setup (dpdk_device_t * xd)  
{  
  dpdk_main_t *dm = &dpdk_main;  
...  
  struct rte_eth_dev_info dev_info;  
...  
  if (xd->flags & DPDK_DEVICE_FLAG_ADMIN_UP)  
    {  
      vnet_hw_interface_set_flags (dm->vnet_main, xd->hw_if_index, 0);  
      dpdk_device_stop (xd);  
    }  
  
  /* Enable flow director when flows exist */  
  if (xd->pmd == VNET_DPDK_PMD_I40E)  
    {  
      if ((xd->flags & DPDK_DEVICE_FLAG_RX_FLOW_OFFLOAD) != 0)  
  xd->port_conf.fdir_conf.mode = RTE_FDIR_MODE_PERFECT;  
      else  
  xd->port_conf.fdir_conf.mode = RTE_FDIR_MODE_NONE;  
    }  
  
  rte_eth_dev_info_get (xd->port_id, &dev_info);
```

**链路状态：**功能：支持获取链路速率、双工模式及链路状态（上/下）。接口：rte_eth_link_get_nowait()

```
/*  
 *Copyright (c) 2015 Cisco and/or its affiliates. Licensed under the Apache License, Version 2.0. You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0  
 */  
  
dpdk_update_link_state (dpdk_device_t * xd, f64 now)  
{  
  vnet_main_t *vnm = vnet_get_main ();  
  struct rte_eth_link prev_link = xd->link;  
...  
  /* only update link state for PMD interfaces */  
  if ((xd->flags & DPDK_DEVICE_FLAG_PMD) == 0)  
    return;  
  
  xd->time_last_link_update = now ? now : xd->time_last_link_update;  
  clib_memset (&xd->link, 0, sizeof (xd->link));  
  rte_eth_link_get_nowait (xd->port_id, &xd->link);
```

**无锁传输队列（Lock-Free Tx Queue）：**若某PMD（Poll Mode Driver）声明支持DEV_TX_OFFLOAD_MT_LOCKFREE特性，则多个线程能够在无需软件锁的情况下，同时对同一传输队列调用rte_eth_tx_burst()函数。API: rte_eth_tx_burst()

**混杂模式：**支持启用/禁用端口的混杂模式。

API：rte_eth_promiscously_enable（），rte_eth_promiscously_disable（），rte_eth_promiscously_get（）

**全组播模式：**支持启用/禁用接收组播帧。

API：rte_eth_allmulticast_enable（），rte_eth_allmulticast_disable（），rte_eth_allmulticast_get（）

```
/*  
 * Copyright (c) 2017 Cisco and/or its affiliates. Licensed under the Apache License, Version 2.0. You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0  
 */  
  
dpdk_device_stop (dpdk_device_t * xd)  
{  
  if (xd->flags & DPDK_DEVICE_FLAG_PMD_INIT_FAIL)  
    return;  
  
  rte_eth_allmulticast_disable (xd->port_id);  
  rte_eth_dev_stop (xd->port_id);  
...
```

**单播MAC过滤器：**支持添加MAC地址以启用白名单过滤以接受数据包。

API：  rte_eth_dev_default_mac_addr_set（），rte_eth_dev_mac_addr_add（），rte_eth_dev_mac_addr_remove（），rte_eth_macaddr_get（）

**VLAN过滤器：**支持对 VLAN Tag 标识符进行过滤。

API：rte_eth_dev_vlan_filter()

**VLAN 卸载：**支持 VLAN 卸载到硬件。

API：rte_eth_dev_set_vlan_offload()、rte_eth_dev_get_vlan_offload()

```
/*  
 * Copyright (c) 2015 Cisco and/or its affiliates. Licensed under the Apache License, Version 2.0. You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0  
 */  
  
dpdk_subif_add_del_function (vnet_main_t * vnm,  
           u32 hw_if_index,  
           struct vnet_sw_interface_t *st, int is_add)  
{  
...  
  dpdk_device_t *xd = vec_elt_at_index (xm->devices, hw->dev_instance);  
  int r, vlan_offload;  
...  
  vlan_offload = rte_eth_dev_get_vlan_offload (xd->port_id);  
  vlan_offload |= ETH_VLAN_FILTER_OFFLOAD;  
  
  if ((r = rte_eth_dev_set_vlan_offload (xd->port_id, vlan_offload)))  
    {  
      xd->num_subifs = prev_subifs;  
      err = clib_error_return (0, "rte_eth_dev_set_vlan_offload[%d]: err %d",  
             xd->port_id, r);  
      goto done;  
    }  
  
  if ((r =  
       rte_eth_dev_vlan_filter (xd->port_id,  
        t->sub.eth.outer_vlan_id, is_add)))  
    {  
      xd->num_subifs = prev_subifs;  
      err = clib_error_return (0, "rte_eth_dev_vlan_filter[%d]: err %d",  
             xd->port_id, r);  
      goto done;  
    }
```


**基本统计数据：**支持基本统计信息，例如：ipackets、opackets、ibytes、obytes、imissed、ierrors、oerrors、rx_nombuf。每个队列统计信息：q_ipackets、q_opackets、q_ibytes、q_obytes、q_errors。

API：rte_eth_stats_get、rte_eth_stats_reset()

```
/*  
 * Copyright (c) 2015 Cisco and/or its affiliates. Licensed under the Apache License, Version 2.0. You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0  
 */  
  
dpdk_update_counters (dpdk_device_t * xd, f64 now)  
{  
  vlib_simple_counter_main_t *cm;  
  vnet_main_t *vnm = vnet_get_main ();  
  u32 thread_index = vlib_get_thread_index ();  
  u64 rxerrors, last_rxerrors;  
  
  /* only update counters for PMD interfaces */  
  if ((xd->flags & DPDK_DEVICE_FLAG_PMD) == 0)  
    return;  
  
  xd->time_last_stats_update = now ? now : xd->time_last_stats_update;  
  clib_memcpy_fast (&xd->last_stats, &xd->stats, sizeof (xd->last_stats));  
  rte_eth_stats_get (xd->port_id, &xd->stats);
```

**扩展统计数据：**支持扩展统计，随驱动程序的变化。

API：  rte_eth_xstats_get（），rte_eth_xstats_reset（），rte_eth_xstats_get_names，rte_eth_xstats_get_by_id（），rte_eth_xstats_get_names_by_id（），rte_eth_xstats_get_id_by_name（）

**模块 EEPROM 转储：**支持获取插件模块eeprom的信息和数据。

API：rte_eth_dev_get_module_info（），rte_eth_dev_get_module_eeprom（）


## Vlib提供了哪些功能？

Vlib是一套面向向量处理的库，同时涵盖了多种应用程序管理功能：

- 缓冲区、内存和图形节点的管理与调度
- 可靠组播支持
- 极轻量级协作式多任务线程
- 物理内存管理及Linux epoll接口支持
- 计数器的维护与导出
- 线程管理
- 数据包追踪

此外，Vlib还实现了调试命令行接口（debug CLI）。在VPP（使用Vlib构建）中，一个向量被定义为vlib_frame_t类型的实例。

```
/*  
 * Copyright (c) 2015 Cisco and/or its affiliates. Licensed under the Apache License, Version 2.0. You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0  
 */  
  
typedef struct vlib_frame_t  
{  
  /* Frame flags. */  
  u16 flags;  
  
  /* Number of scalar bytes in arguments. */  
  u8 scalar_size;  
  
  /* Number of bytes per vector argument. */  
  u8 vector_size;  
  
  /* Number of vector elements currently in frame. */  
  u16 n_vectors;  
  
  /* Scalar and vector arguments to next node. */  
  u8 arguments[0];  
} vlib_frame_t;
```

如图所示，向量实质上是一种带有用户自定义“头部”的动态大小数组。VPP中许多数据结构（如缓冲区、哈希表、堆、内存池等）均是以不同头部形式存在的向量。其内存布局如下所示：

```
© Copyright 2018, Linux Foundation  
  
  
User header (optional, uword aligned)  
                  Alignment padding (if needed)  
                  Vector length in elements  
User's pointer -> Vector element 0  
                  Vector element 1  
                  ...  
                  Vector element N-1
```

向量不仅应用于VPP基础设施（vppinfra）中的数据结构（如哈希表、堆、内存池等），还在Vlib内部扮演重要角色，涉及节点、缓冲区、进程等多个方面。vppinfra基础设置的数据结构可以参考公众号文章[《vppinfra系列解读》](https://mp.weixin.qq.com/mp/appmsgalbum?__biz=MzI2MTcxMDE2MQ==&action=getalbum&album_id=1857454855896760324&scene=173&subscene=&sessionid=svr_c316ee06758&enterid=1713055288&from_msgid=2247483845&from_itemidx=1&count=3&nolastread=1#wechat_redirect)。

**缓冲区（Buffers）：**Vlib缓冲区的设计旨在实现高速的包处理性能。为此，系统采用一次性分配/释放N个缓冲区而非逐个处理的方式。除了在特定节点直接处理个别缓冲区（及其承载的数据包）时，通常使用缓冲区索引而非指针进行操作。Vlib缓冲区具有向量的结构特征：

```
/*  
 * Copyright (c) 2015 Cisco and/or its affiliates. Licensed under the Apache License, Version 2.0. You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0  
 */  
  
/** VLIB buffer representation. */  
typedef union  
{  
  struct  
  {  
    CLIB_CACHE_LINE_ALIGN_MARK (cacheline0);  
  
    /** signed offset in data[], pre_data[] that we are currently  
      * processing. If negative current header points into predata area.  */  
    i16 current_data;  
  
    /** Nbytes between current data and the end of this buffer.  */  
    u16 current_length;  
...  
    /** Opaque data used by sub-graphs for their own purposes. */  
    u32 opaque[10];  
...  
    /**< More opaque data, see ../vnet/vnet/buffer.h */  
    u32 opaque2[14];  
  
    /** start of third cache line */  
      CLIB_CACHE_LINE_ALIGN_MARK (cacheline2);  
  
    /** Space for inserting data before buffer start.  Packet rewrite string  
      * will be rewritten backwards and may extend back before  
      * buffer->data[0].  Must come directly before packet data.  */  
    u8 pre_data[VLIB_BUFFER_PRE_DATA_SIZE];  
  
    /** Packet data */  
    u8 data[0];  
  };  
#ifdef CLIB_HAVE_VEC128  
  u8x16 as_u8x16[4];  
#endif  
#ifdef CLIB_HAVE_VEC256  
  u8x32 as_u8x32[2];  
#endif  
#ifdef CLIB_HAVE_VEC512  
  u8x64 as_u8x64[1];  
#endif  
} vlib_buffer_t;
```

每个vlib_buffer_t（数据包缓冲区）携带了描述当前数据包处理状态的缓冲区元数据。在公众号文章《[vpp Buffer Metadata](http://mp.weixin.qq.com/s?__biz=MzI2MTcxMDE2MQ==&mid=2247483849&idx=1&sn=b511a7ddd85a5bbbdcba9a457a55e1d2&chksm=ea5709f7dd2080e18692f5a68bd36658a30afff7ea2f46a00a2a4d83fefeda8760fcff58355f&scene=21#wechat_redirect)》中详细介绍各个字段的描述。

- u8 data[0]：通常情况下，硬件设备将data作为DMA目标，但也有例外情况。请勿直接访问data，应使用vlib_buffer_get_current函数。
- u32 opaque[10]：主要的vnet层不透明数据
- u32 opaque2[14]：次要的vnet层不透明数据

有若干函数用于从向量中获取数据（位于vlib/node_funcs.h文件中）：

**获取指向帧矢量数据的指针**

```
always_inline void *  
vlib_frame_vector_args (vlib_frame_t * f)  
{  
  return (void *) f + vlib_frame_vector_byte_offset (f->scalar_size);  
}  
- to get pointer to scalar data  
always_inline void *  
vlib_frame_scalar_args (vlib_frame_t * f)  
{  
  return vlib_frame_vector_args (f) - f->scalar_size;  
}
```

**获取指向标量数据的指针**

```
always_inline void *  
vlib_frame_scalar_args (vlib_frame_t * f)  
{  
  return vlib_frame_vector_args (f) - f->scalar_size;  
}
```

**将缓冲区索引转换为缓冲区指针**

```
always_inline vlib_buffer_t *  
vlib_get_buffer (vlib_main_t * vm, u32 buffer_index)  
{  
  vlib_buffer_main_t *bm = vm->buffer_main;  
  vlib_buffer_t *b;  
  
  b = vlib_buffer_ptr_from_index (bm->buffer_mem_start, buffer_index, 0);  
  vlib_buffer_validate (vm, b);  
  return b;  
}
```

**从缓冲区获取指向当前（数据包）数据的指针以进行处理**

```
always_inline void *  
vlib_buffer_get_current (vlib_buffer_t * b)  
{  
  /* Check bounds. */  
  ASSERT ((signed) b->current_data >= (signed) -VLIB_BUFFER_PRE_DATA_SIZE);  
  return b->data + b->current_data;
```

**在保留的不透明字段中获取 vnet 主缓冲区元数据**

```
#define vnet_buffer(b) ((vnet_buffer_opaque_t *) (b)->opaque)
```

**检索vnet缓冲区数据的示例：**

```
add_to_flow_record_state (vlib_main_t * vm, vlib_node_runtime_t * node,  
        flowprobe_main_t * fm, vlib_buffer_t * b,  
        timestamp_nsec_t timestamp, u16 length,  
        flowprobe_variant_t which, flowprobe_trace_t * t)  
{  
...  
  u32 rx_sw_if_index = vnet_buffer (b)->sw_if_index[VLIB_RX];
```

**在保****留的 opaque2 字段中获取 vnet 主缓冲区元数据**

```
#define vnet_buffer2(b) ((vnet_buffer_opaque2_t *) (b)->opaque2)
```

我们来看看flowprobe节点的处理函数。Vlib函数始终以vlib_前缀开头

```
/*  
 * Copyright (c) 2017 Cisco and/or its affiliates. Licensed under the Apache License, Version 2.0. You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0  
 */  
  
flowprobe_node_fn (vlib_main_t * vm,  
       vlib_node_runtime_t * node, vlib_frame_t * frame,  
       flowprobe_variant_t which)  
{  
  u32 n_left_from, *from, *to_next;  
  flowprobe_next_t next_index;  
  flowprobe_main_t *fm = &flowprobe_main;  
  timestamp_nsec_t timestamp;  
  
  unix_time_now_nsec_fraction (&timestamp.sec, &timestamp.nsec);  
  
// access frame vector data  
  from = vlib_frame_vector_args (frame);  
  n_left_from = frame->n_vectors;  
  next_index = node->cached_next_index;  
  
  while (n_left_from > 0)  
    {  
      u32 n_left_to_next;  
  
      // get pointer to next vector data  
      vlib_get_next_frame (vm, node, next_index, to_next, n_left_to_next);  
  
// dual loop – we are processing two buffers and prefetching next two buffers  
      while (n_left_from >= 4 && n_left_to_next >= 2)  
  {  
    u32 next0 = FLOWPROBE_NEXT_DROP;  
    u32 next1 = FLOWPROBE_NEXT_DROP;  
    u16 len0, len1;  
    u32 bi0, bi1;  
    vlib_buffer_t *b0, *b1;  
  
    /* Prefetch next iteration. */  
             // prefetching packets p3 and p4 while p1 and p2 are processed  
    {  
      vlib_buffer_t *p2, *p3;  
  
      p2 = vlib_get_buffer (vm, from[2]);  
      p3 = vlib_get_buffer (vm, from[3]);  
  
      vlib_prefetch_buffer_header (p2, LOAD);  
      vlib_prefetch_buffer_header (p3, LOAD);  
  
      CLIB_PREFETCH (p2->data, CLIB_CACHE_LINE_BYTES, STORE);  
      CLIB_PREFETCH (p3->data, CLIB_CACHE_LINE_BYTES, STORE);  
    }  
/* speculatively enqueue b0 and b1 to the current next frame */  
// frame contains buffer indecies (bi0, bi1) instead of pointers  
    to_next[0] = bi0 = from[0];  
    to_next[1] = bi1 = from[1];  
    from += 2;  
    to_next += 2;  
    n_left_from -= 2;  
    n_left_to_next -= 2;  
  
// translate buffer index to buffer pointer  
    b0 = vlib_get_buffer (vm, bi0);  
    b1 = vlib_get_buffer (vm, bi1);  
// select next node based on feature arc  
    vnet_feature_next (&next0, b0);  
    vnet_feature_next (&next1, b1);  
  
    len0 = vlib_buffer_length_in_chain (vm, b0);  
// get current data (header) from packet to process  
// currently we are on L2 so get etehernet header, but if we  
// are on L3 for example we can retrieve L3 header, i.e.  
// ip4_header_t *ip0 = (ip4_header_t *) ((u8 *) vlib_buffer_get_current (b0)   
    ethernet_header_t *eh0 = vlib_buffer_get_current (b0);  
    u16 ethertype0 = clib_net_to_host_u16 (eh0->type);  
  
    if (PREDICT_TRUE ((b0->flags & VNET_BUFFER_F_FLOW_REPORT) == 0))  
      add_to_flow_record_state (vm, node, fm, b0, timestamp, len0,  
              flowprobe_get_variant  
              (which, fm->context[which].flags,  
               ethertype0), 0);  
...  
/* verify speculative enqueue, maybe switch current next frame */  
    vlib_validate_buffer_enqueue_x1 (vm, node, next_index,  
             to_next, n_left_to_next,  
             bi0, next0);  
  }  
  
      vlib_put_next_frame (vm, node, next_index, n_left_to_next);  
    }  
  return frame->n_vectors;  
}
```


**NODE**

如前所述，vlib 也被设计用于图节点管理。在创建新特性时，必须使用 VLIB_INIT_FUNCTION 宏对其进行初始化。这将构建一个 vlib_node_registration_t 结构体，通常通过 VLIB_REGISTER_NODE 宏实现。在运行时，框架会将这一系列注册信息处理成一个有向图。

```
static clib_error_t *  
flowprobe_init (vlib_main_t * vm)  
{  
  /* ... initialize things ... */  
  
  return 0;  
}  
  
VLIB_INIT_FUNCTION (flowprobe_init);  
  
...  
  
VLIB_REGISTER_NODE (flowprobe_l2_node) = {  
  .function = flowprobe_l2_node_fn,  
  .name = "flowprobe-l2",  
  .vector_size = sizeof (u32),  
  .format_trace = format_flowprobe_trace,  
  .type = VLIB_NODE_TYPE_INTERNAL,  
  .n_errors = ARRAY_LEN(flowprobe_error_strings),  
  .error_strings = flowprobe_error_strings,  
  .n_next_nodes = FLOWPROBE_N_NEXT,  
  .next_nodes = FLOWPROBE_NEXT_NODES,  
};  
  
VLIB_REGISTER_NODE (flowprobe_walker_node) = {  
  .function = flowprobe_walker_process,  
  .name = "flowprobe-walker",  
  .type = VLIB_NODE_TYPE_INPUT,  
  .state = VLIB_NODE_STATE_INTERRUPT,  
};
```

**节点注册中的类型成员**指定节点的用途（在公众号文章《[learning:vpp node type](http://mp.weixin.qq.com/s?__biz=MzI2MTcxMDE2MQ==&mid=2247484691&idx=1&sn=31cf20e3e1d05c15e58675bbfb5e047b&chksm=ea570d2ddd20843b0344b5657c5eb399658f877cd65c1f058b88f4b39e0c74a551e6d017b182&scene=21#wechat_redirect)》有详细解读过node类型）：

- VLIB_NODE_TYPE_PRE_INPUT – 在所有其他节点类型之前运行
- VLIB_NODE_TYPE_INPUT – 在 pre_input 节点之后尽可能频繁地运行
- VLIB_NODE_TYPE_INTERNAL – 仅当通过添加待处理帧显式使其可运行时
- VLIB_NODE_TYPE_PROCESS – 仅当显式使其可运行时。

功能的初始化在应用程序启动时的某个时刻执行。但是，必须使用约束来指定顺序（当一个功能必须在另一功能之后/之前初始化时）。可以使用VNET_FEATURE_INT宏将功能挂钩到特定功能arc feature。

```
VNET_FEATURE_INIT (ip4_nat44_ed_hairpin_src, static) = {  
  .arc_name = "ip4-output",  
  .node_name = "nat44-ed-hairpin-src",  
  .runs_after = VNET_FEATURES ("acl-plugin-out-ip4-fa"),  
};  
  
VNET_FEATURE_INIT (ip4_nat_hairpinning, static) =  
{  
  .arc_name = "ip4-local",  
  .node_name = "nat44-hairpinning",  
  .runs_before = VNET_FEATURES("ip4-local-end-of-arc"),  
};
```

由于VLIB_NODE_TYPE_INPUT节点是功能arc feature的起点，因此它们负责从某个源（例如 NIC 或 PCAP 文件）生成数据包，并将它们注入图的其余部分。

注册节点时，可以提供.next_node参数以及图中即将到来的节点的索引列表。例如下面的flowprobe节点：

```
...  
next_nodes = FLOWPROBE_NEXT_NODES,  
...  
  
#define FLOWPROBE_NEXT_NODES {    \  
    [FLOWPROBE_NEXT_DROP] = "error-drop",  \  
    [FLOWPROBE_NEXT_IP4_LOOKUP] = "ip4-lookup",  \  
}
```

_vnet_feature_next_通常用于选择下一个节点。这种选择基于arc feature机制（在文章《[VPP feature arc机制](http://mp.weixin.qq.com/s?__biz=MzI2MTcxMDE2MQ==&mid=2247484095&idx=1&sn=0e021ea6df37919ea7bab88c6d751429&chksm=ea570a81dd208397f765b728200ae69e35d4383912f8107dab9f7ba905e9a0f86ed8d3b2deec&scene=21#wechat_redirect)》中解读过），如上面的_flowprobe_示例所示：

```
flowprobe_node_fn (vlib_main_t * vm,  
       vlib_node_runtime_t * node, vlib_frame_t * frame,  
       flowprobe_variant_t which)  
{  
...  
    b0 = vlib_get_buffer (vm, bi0);  
    b1 = vlib_get_buffer (vm, bi1);  
        // select next node based on feature arc  
    vnet_feature_next (&next0, b0);  
    vnet_feature_next (&next1, b1);
```

图节点调度器将工作向量沿有向图推进，按需进行细分，直到原始工作向量被完全处理。

图节点调度函数通过调用vlib_get_next_frame来设置(u32 *)to_next指针，使其指向vlib_frame_t结构中对应位置。该位置与当前节点的第i条弧（即next0）到指定下一节点相对应。

在调度函数返回前，必须对其实际使用的图中所有弧调用vlib_put_next_frame。此举会向图调度器的待处理帧向量中添加一个vlib_pending_frame_t实例。

```
vlib_put_next_frame (vm, node, next_index, n_left_to_next);  
    }  
  return frame->n_vectors;  
}
```