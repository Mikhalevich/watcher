#ifndef QUERRYFACTORY_H
#define QUERRYFACTORY_H

#include <map>
#include <stdexcept>

#include <QSharedPointer>

template <typename Base>
class AbstractCreator
{
public:
    virtual ~AbstractCreator()
    {
    }

    virtual QSharedPointer<Base> create() const = 0;
};
//////////////////////////////////////////////////

template <typename Base, typename Derrived>
class Creator : public AbstractCreator<Base>
{
public:
    virtual QSharedPointer<Base> create() const
    {
        return QSharedPointer<Base>(new Derrived());
    }
};
//////////////////////////////////////////////////

template <typename Key, typename Base>
class ObjectFactory
{
private:
    typedef AbstractCreator<Base> AbstractBaseCreator;
    typedef std::map<Key, AbstractBaseCreator*> FactoryMap;

public:
    ~ObjectFactory();

    template <typename Derrived>
    void add(const Key& key);

    QSharedPointer<Base> create(const Key& key) const;

private:
    FactoryMap factoryMap_;
};
////////////////////////////////////////////////////

// ObjectFactory implementation

template <typename Key, typename Base>
ObjectFactory<Key, Base>::~ObjectFactory()
{
    // TODO
    // deleate all elements
}

template <typename Key, typename Base>
template <typename Derrived>
void ObjectFactory<Key, Base>::add(const Key &key)
{
    typename FactoryMap::const_iterator it = factoryMap_.find(key);
    if (it == factoryMap_.end())
    {
        std::pair<Key, AbstractBaseCreator*> pair(key, new Creator<Base, Derrived>());
        factoryMap_.insert(pair);
    }
    else
    {
        // same element exists in factory
        throw std::runtime_error("The same elements exists in factory");
    }
}

template <typename Key, typename Base>
QSharedPointer<Base> ObjectFactory<Key, Base>::create(const Key &key) const
{
    typename FactoryMap::const_iterator it = factoryMap_.find(key);
    if (it != factoryMap_.end())
    {
        return it->second->create();
    }
    else
    {
        // ther is no such element
        throw std::runtime_error("Ther is no such element in factory");
    }
}

#endif // QUERRYFACTORY_H
