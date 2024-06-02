#include <iostream>
#include <thread>
#include <csignal>
#include <cstring>
#include <string>
#include <chrono>
#include <librdkafka/rdkafkacpp.h>

constexpr const char *KAFKA_BROKERS = "192.168.239.50:9092";
constexpr const char *KAFKA_TOPIC = "test_topic";
constexpr const char *KAFKA_GROUP = "test_group";

// 信号处理
static bool run = true;
static void sigterm(int sig)
{
    run = false;
}

// 生产者示例函数
void produce_message(const std::string &brokers, const std::string &topic)
{
    std::string errstr;
    std::string message = "Hello Kafka!";

    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);

    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
    if (!producer)
    {
        std::cerr << "Failed to create producer: " << errstr << std::endl;
        exit(1);
    }

    RdKafka::Topic *topic_obj = RdKafka::Topic::create(producer, topic, nullptr, errstr);
    if (!topic_obj)
    {
        std::cerr << "Failed to create topic: " << errstr << std::endl;
        exit(1);
    }

    for (int i = 0; i < 10; ++i)
    {
        RdKafka::ErrorCode resp = producer->produce(topic_obj, RdKafka::Topic::PARTITION_UA,
                                                    RdKafka::Producer::RK_MSG_COPY,
                                                    const_cast<char *>(message.c_str()), message.size(),
                                                    nullptr, nullptr);
        if (resp != RdKafka::ERR_NO_ERROR)
        {
            std::cerr << "Failed to produce message: " << RdKafka::err2str(resp) << std::endl;
        }
        else
        {
            std::cout << "Produced message: " << message << std::endl;
        }

        producer->poll(0);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    producer->flush(10000);
    delete topic_obj;
    delete producer;
    delete conf;
}

// 消费者示例函数
void consume_message(const std::string &brokers, const std::string &topic, const std::string &group_id)
{
    std::string errstr;

    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);
    conf->set("group.id", group_id, errstr);
    conf->set("auto.offset.reset", "earliest", errstr);

    RdKafka::KafkaConsumer *consumer = RdKafka::KafkaConsumer::create(conf, errstr);
    if (!consumer)
    {
        std::cerr << "Failed to create consumer: " << errstr << std::endl;
        exit(1);
    }

    std::vector<std::string> topics = {topic};
    consumer->subscribe(topics);

    while (run)
    {
        RdKafka::Message *msg = consumer->consume(1000);
        switch (msg->err())
        {
        case RdKafka::ERR__TIMED_OUT:
            break;
        case RdKafka::ERR_NO_ERROR:
            std::cout << "Received message: " << std::string(static_cast<const char *>(msg->payload()), msg->len()) << std::endl;
            break;
        case RdKafka::ERR__PARTITION_EOF:
            std::cerr << "End of partition reached" << std::endl;
            break;
        case RdKafka::ERR__UNKNOWN_TOPIC:
        case RdKafka::ERR__UNKNOWN_PARTITION:
            std::cerr << "Consume error: " << msg->errstr() << std::endl;
            run = false;
            break;
        default:
            std::cerr << "Consume error: " << msg->errstr() << std::endl;
            run = false;
        }
        delete msg;
    }

    consumer->close();
    delete consumer;
    delete conf;
}

int main(int argc, char **argv)
{
    // docker run -d --name kafka -p 9092:9092 -e ALLOW_PLAINTEXT_LISTENER=yes -e KAFKA_CFG_AUTO_CREATE_TOPICS_ENABLE=true docker.io/bitnami/kafka:3.0.0
    std::string brokers = KAFKA_BROKERS;
    std::string topic = KAFKA_TOPIC;
    std::string group_id = KAFKA_GROUP;

    signal(SIGINT, sigterm);
    signal(SIGTERM, sigterm);

    // 启动生产者和消费者
    std::thread producer_thread(produce_message, brokers, topic);
    std::thread consumer_thread(consume_message, brokers, topic, group_id);

    producer_thread.join();
    consumer_thread.join();

    RdKafka::wait_destroyed(5000);
    return 0;
}