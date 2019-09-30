#include "stdio.h"
#include "string.h"
#include "pthread.h"
#include "stdlib.h"
#include "signal.h"
#include "pipeLine.h"

typedef struct StationA {
  Istation station;
  int a;
} StationA;

typedef struct StationB {
  Istation station;
  int b;
} StationB;

typedef struct StationC {
  Istation station;
  int c;
} StationC;

StationA *sa = NULL;
StationB *sb = NULL;
StationC *sc = NULL;
/*sa*/
static void _pd_connect_cs(Istation *pd, Istation *cs) {
  if (pd->link_consumers_num == MAX_NBR_CONSUMER) {
    printf("sa consumers too many %d.\n", pd->link_consumers_num);
    return ;
  }
  int i;
  for (i = 0; i < MAX_NBR_CONSUMER; i ++) {
    if (pd->link_consumers[i] == NULL) {
      pd->link_consumers[i] = cs;
      pd->link_consumers_num ++;
      printf("%s link consumer suc\n", pd->name);
      break;
    }
  }
}

static void _pd_disconnect_cs(Istation *pd, Istation *cs) {
  int i;
  for (i = 0; i < MAX_NBR_CONSUMER; i ++) {
    if (pd->link_consumers[i] == cs) {
      pd->link_consumers[i] = NULL;
      pd->link_consumers_num --;
      printf("%s break consumer suc\n", pd->name);
      break;
    }
  }
}

static void _pd_clear_cs(Istation *pd) {
  int i;
  for(i = 0; i < MAX_NBR_CONSUMER; i ++) {
    pd->link_consumers[i] = NULL;
  }
  pd->link_consumers_num = 0;
  printf("%s clear consumer\n", pd->name);
}

static int _consumer_acquist(Istation *consumer, void *data, int size) {
  int i;
  for (i = 0; i < MAX_NBR_CONSUMER; i ++) {
    if (consumer->link_consumers[i] != NULL) {
      printf("%s station acquist %d(%d).\n", consumer->name, *((int *)data), size);
      Istation *cs = consumer->link_consumers[i];
      cs->consumer.consumer_acquist(cs, data, size);
    }
  }
  return 0;
}

static int _consumer_event(Istation *consumer, void *event) {
  int i;
  for (i = 0; i < MAX_NBR_CONSUMER; i ++) {
    if (consumer->link_consumers[i] != NULL) {
      printf("%s station event: %s.\n", consumer->name, (char *)event);
      Istation *cs = consumer->link_consumers[i];
      cs->consumer.consumer_event(cs, event);
    }
  }
  return 0;
}

static int _consumer_start(Istation *consumer, void *args) {
  int i;
  for (i = 0; i < MAX_NBR_CONSUMER; i ++) {
    if (consumer->link_consumers[i] != NULL) {
      printf("%s station start: %s.\n", consumer->name, (char *)args);
      Istation *cs = consumer->link_consumers[i];
      cs->consumer.consumer_event(cs, args);
    }
  }
  return 0;
}

static int _sc_consumer_acquist(Istation *consumer, void *data, int size) {
  printf("%s recive data:%d,size:%d.\n", consumer->name, *((int *)data), size);

  return 0;
}

static int _sc_consumer_start(Istation *consumer, void *args) {
  printf("%s recive args:%s.\n", consumer->name, (char *)args);
  return 0;
}

static int _sc_consumer_event(Istation *consumer, void *event) {
  printf("%s recive event:%s.\n", consumer->name, (char *)event);
  return 0;
}

static void *_sa_thread(void *argv) {
  StationA *sa = (StationA *)argv;
  while (1) {
    printf("sa start\n");
    NotifyConsumersStart(&sa->station, "sa-start");
    sleep(5);
    printf("sa event\n");
    NotifyConsumerEvent(&sa->station, "sa-event");
    sleep(5);
    NotifyConsumerData(&sa->station, (void *)&sa->a, sizeof(int));
    sa->a ++;
    sleep(10);
  }
}

static void _sig_usr1(int sig) {
  printf("link pd cs\n");
  ConnectProducerConsumer(&sa->station, &sb->station);
  ConnectProducerConsumer(&sb->station, &sc->station);
}

static void _sig_usr2(int sig) {
  printf("unlink pd cs\n");
  DisconnectProducerComsumer(&sa->station, &sb->station);
  DisconnectProducerComsumer(&sb->station, &sc->station);
}

static void _init_signal(void) {
  signal(SIGUSR1, _sig_usr1);
  signal(SIGUSR2, _sig_usr2);
}

int main(int argc, char *argv[]) {
  printf("start @@@\n");
  /* sa */
  sa = (StationA *)malloc(sizeof(StationA));
  memset(sa, 0, sizeof(StationA));
  SetStationName(&sa->station, "sa");
  RegisterProducerHandler(&sa->station, _pd_connect_cs, _pd_disconnect_cs, _pd_clear_cs);
  RegisterConsumerHandler(&sa->station, _consumer_acquist, _consumer_event, _consumer_start);
  pthread_t  sa_id;
  pthread_create(&sa_id, NULL, _sa_thread, (void *)sa);
  pthread_detach(sa_id);

  /*sb*/
  sb = (StationB *)malloc(sizeof(StationB));
  memset(sb, 0, sizeof(StationB));
  SetStationName(&sb->station, "sb");
  RegisterProducerHandler(&sb->station, _pd_connect_cs, _pd_disconnect_cs, _pd_clear_cs);
  RegisterConsumerHandler(&sb->station, _consumer_acquist, _consumer_event, _consumer_start);

  /*sc*/
  sc = (StationC *)malloc(sizeof(StationC));
  memset(sc, 0, sizeof(StationC));
  SetStationName(&sc->station, "sc");
  RegisterProducerHandler(&sc->station, _pd_connect_cs, _pd_disconnect_cs, _pd_clear_cs);
  RegisterConsumerHandler(&sc->station, _sc_consumer_acquist, _sc_consumer_event, _sc_consumer_start);
  _init_signal();
  while(1) {
    sleep(5);
  }
  return 0;
}