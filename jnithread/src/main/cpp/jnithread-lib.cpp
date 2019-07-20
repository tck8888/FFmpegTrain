#include <jni.h>
#include <string>

#include "pthread.h"

#include "AndroidLog.h"

pthread_t thread;

void *normalCallBack(void *data) {
    LOGD("create normal thread from C++!");
    pthread_exit(&thread);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tck_jnithread_JNIThread_normalThread(JNIEnv *env, jobject instance) {

    pthread_create(&thread, NULL, normalCallBack, NULL);
}

#include "queue"
#include "unistd.h"

pthread_t producer;
pthread_t consumer;

//用于创建线程锁对象
pthread_mutex_t mutex;
//用于创建线程条件对象
pthread_cond_t cond;

std::queue<int> queue;

bool isExit = true;

void *produce(void *data) {
    while (isExit) {
        pthread_mutex_lock(&mutex);
        queue.push(1);
        LOGD("生产者生产一个产品，通知消费者消费， 产品数量为 %d", queue.size());
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        sleep(5);
    }

    LOGD("生产者退出");
    pthread_exit(&producer);
}

void *consume(void *data) {
    while (isExit) {
        pthread_mutex_lock(&mutex);
        if (queue.size() > 0) {
            queue.pop();
            LOGD("消费者消费产品，产品数量还剩余 %d ", queue.size());
        } else {
            LOGD("没有产品可以消费， 等待中...");
            pthread_cond_wait(&cond, &mutex);
        }

        pthread_mutex_unlock(&mutex);

        usleep(500 * 1000);
    }

    LOGD("消费者退出");
    pthread_exit(&consumer);

}


extern "C"
JNIEXPORT void JNICALL
Java_com_tck_jnithread_JNIThread_mutexThread(JNIEnv *env, jobject instance) {
    for (int i = 0; i < 10; i++) {
        queue.push(i);
    }

    pthread_mutex_init(&mutex, NULL);

    pthread_cond_init(&cond, NULL);

    pthread_create(&producer, NULL, produce, NULL);

    pthread_create(&consumer, NULL, consume, NULL);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_tck_jnithread_JNIThread_stopMutexThread(JNIEnv *env, jobject instance) {
    isExit = false;
}


#include "JavaListener.h"

JavaVM *jvm;

JavaListener *javaListener;

pthread_t chidlThread;


void *childCallback(void *data)
{
    JavaListener *javaListener1 = (JavaListener *) data;

    javaListener1->onError(0, 101, "c++ call java meid from child thread!");
    pthread_exit(&chidlThread);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tck_jnithread_JNIThread_callbackFromC(JNIEnv *env, jobject instance) {
    javaListener = new JavaListener(jvm, env, env->NewGlobalRef(instance));
   // javaListener->onError(1, 100, "c++ call java meid from main thread!");

    pthread_create(&chidlThread, NULL, childCallback, javaListener);
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGD("JNI_OnLoad");
    JNIEnv *env;
    jvm = vm;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}

