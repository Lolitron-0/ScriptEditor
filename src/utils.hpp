#ifndef UTITITY_HPP
#define UTITITY_HPP

#include <QDataStream>



template<class T> class Singleton {
public:
    static T& getInstance() {
        if (!Singleton::instance)
            Singleton::instance = createInstance();
        return *(Singleton::instance);
    }

protected:
    inline explicit Singleton() {
        assert(!Singleton::instance);
        Singleton::instance = static_cast<T*>(this);
    }
    inline ~Singleton() {
        delete Singleton::instance;
    }

    static T* createInstance() {return new T();}

private:
    static T* instance;

    inline explicit Singleton(const Singleton&) = default;
    inline Singleton& operator=(const Singleton&) {return *this;};
};

template<class T>
T* Singleton<T>::instance = 0;


class IdGenerator : public Singleton<IdGenerator> {
public:
    int getNextId() {
        return (id++);
    }
private:
    int id{0x01};

    friend QDataStream& operator<<(QDataStream& out, const IdGenerator& generator) {
        out << generator.id;
        return out;
    }
    friend QDataStream& operator>>(QDataStream& in, IdGenerator& generator) {
        in >> generator.id;
        return in;
    }
};


#endif // UTITITY_HPP
