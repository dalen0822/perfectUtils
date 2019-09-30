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
 * Description : pipeLine.c
 * Author      : xiaoC
 * Date        : 2019.09.30
 *
 **************************************************************************/
#include "pipeLine.h"
#include "string.h"
#include "stdio.h"

/* resgister producer action*/
void RegisterProducerHandler(Istation *station, ProducerConnectConsumer connect_cb,
                                               ProducerDisconnectConsumer disconnect_cb,
                                               ProducerClearConsumer clear_cb) {
  station->producer.producer_connect_consumer = connect_cb;
  station->producer.producer_disconnect_consumer = disconnect_cb;
  station->producer.producer_clear_consumer = clear_cb;
}

/* resgister consumer action*/
void RegisterConsumerHandler(Istation *station, ConsumerAcquist acquist_cb,
                                       ConsumerEvent event_cb,
                                       ConsumerStart start_cb) {
  station->consumer.consumer_acquist = acquist_cb;
  station->consumer.consumer_event = event_cb;
  station->consumer.consumer_start = start_cb;
}

void SetStationName(Istation *station, const char *name) {
  strncpy(station->name, name, sizeof(station->name) - 1);
}

void ConnectProducerConsumer(Istation *pd_station, Istation *cs_station) {
  pd_station->producer.producer_connect_consumer(pd_station, cs_station);
}

void DisconnectProducerComsumer(Istation *pd_station, Istation *cs_station) {
  pd_station->producer.producer_disconnect_consumer(pd_station, cs_station);
}

void ClearProducerConsumer(Istation *pd_station) {
  pd_station->producer.producer_clear_consumer(pd_station);
}

void NotifyConsumersStart(Istation *pd_station, void *args) {
  int i;
  for (i = 0; i < pd_station->link_consumers_num; i ++) {
    Istation *cs_station = pd_station->link_consumers[i];
    if (cs_station->consumer.consumer_start(cs_station, args) != 0) {
      printf("notify consumer:%s start failed!\n", cs_station->name);
    }
  }
}

void NotifyConsumerEvent(Istation *pd_station, void *event) {
  int i;
  for (i = 0; i < pd_station->link_consumers_num; i ++) {
    Istation *cs_station = pd_station->link_consumers[i];
    if (cs_station->consumer.consumer_event(cs_station, event) != 0) {
      printf("notify consumer:%s event failed!\n", cs_station->name);
    }
  }
}

void NotifyConsumerData(Istation *pd_station, void *data, int size) {
  int i;
  for (i = 0; i < pd_station->link_consumers_num; i ++) {
    Istation *cs_station = pd_station->link_consumers[i];
    if (cs_station->consumer.consumer_acquist(cs_station, data, size) != 0) {
      printf("notify consumer:%s data failed!\n", cs_station->name);
    }
  }
}

