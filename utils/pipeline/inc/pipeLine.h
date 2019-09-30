/**************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **************************************************************************
 *
 * Description : pipeLine.h
 * Author      : xiaoC
 * Date        : 2019.09.30
 *
 **************************************************************************/
#ifndef _INC_PIPELINE_H_
#define _INC_PIPELINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NBR_CONSUMER    (5)

struct Istation;
typedef void (*ProducerConnectConsumer)(struct Istation *produer,
                                        struct Istation *consumer);
typedef void (*ProducerDisconnectConsumer)(struct Istation *produer,
                                           struct Istation *consumer);
typedef void (*ProducerClearConsumer)(struct Istation *produer);

typedef struct Iproducer {
  ProducerConnectConsumer    producer_connect_consumer;
  ProducerDisconnectConsumer producer_disconnect_consumer;
  ProducerClearConsumer      producer_clear_consumer;
} Iproducer;


typedef int (*ConsumerAcquist)(struct Istation *consumer, void *data, int size);
typedef int (*ConsumerStart)(struct Istation *consumer, void *args);
typedef int (*ConsumerEvent)(struct Istation *consumer, void *event);

typedef struct Iconsumer {
  ConsumerAcquist       consumer_acquist;
  ConsumerStart         consumer_start;
  ConsumerEvent         consumer_event;
} Iconsumer;

typedef struct Istation {
  char              name[16];
  Iproducer         producer;
  Iconsumer         consumer;
  struct Istation   *link_consumers[MAX_NBR_CONSUMER];
  int               link_consumers_num;
} Istation;

void RegisterProducerHandler(Istation *station, ProducerConnectConsumer connect_cb,
                                      ProducerDisconnectConsumer disconnect_cb,
                                      ProducerClearConsumer clear_cb);
void RegisterConsumerHandler(Istation *station, ConsumerAcquist acquist_cb,
                                       ConsumerEvent event_cb,
                                       ConsumerStart start_cb);
void SetStationName(Istation *station, const char *name);
void ConnectProducerConsumer(Istation *pd_station, Istation *cs_station);
void DisconnectProducerComsumer(Istation *pd_station, Istation *cs_station);
void ClearProducerConsumer(Istation *pd_station);
void NotifyConsumersStart(Istation *pd_station, void *args);
void NotifyConsumerEvent(Istation *pd_station, void *event);
void NotifyConsumerData(Istation *pd_station, void *data, int size);

#ifdef __cplusplus
}
#endif
#endif
