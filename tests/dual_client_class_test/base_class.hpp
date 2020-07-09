#ifndef BASE_CLASS_HPP
#define BASE_CLASS_HPP

class base_class{
public:
    base_class(char *t);
    void publish();
    void subscribe();
    void on_connect();
    inline const char * topic(){
        return _topic;
    }
private:
    char *_sensor_array;
    const char *_topic;
    int _sensor_qty;
};

#endif
