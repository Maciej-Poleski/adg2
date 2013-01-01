/*
 * Copyright 2013  Maciej Poleski
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <mutex>

#include <boost/serialization/access.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

#include "../shared/Request.hxx"
#include "../shared/Address.hxx"

/**
 * Obiekt bazy danych Mastera
 */
class Database
{
    friend boost::serialization::access;

public:
    /**
     * Tworzy nowe dyskusje na serwerze i przypisuje ich obsługę do Slave
     *
     * @param newDiscussions lista nowych dyskusji do utworzenia
     * @return lista identyfikatorów nowo utworzonych dyskusji (lub 0)
     * wraz z adresami slave odpowiedzialnych za ich obsługę
     */
    std::vector<std::pair<DiscussionId,Address>>
            createNewDiscussions(std::vector< std::string > newDiscussions);

    /**
     * @return obecna wersja listy dyskusji
     */
    DiscussionListVersion currentDiscussionListVersion() const;

    /**
     * Rejestruje nowego Slave w tym Master'ze
     *
     * @param address adres slave
     */
    void registerSlave(const Address &address);

private:
    /**
     * @return Slave który otrzyma nowe zadanie
     */
    Address selectSlave();

private:
    template<class Action>
    void serialize(Action &ar, const unsigned int version)
    {
        ar & _discussionNames & _discussionSlaves &
        _discussionListVersionChangeset & _nextDiscussionId &
        _nextDiscussionListVersion & _slaves;
    }

private:
    std::map<DiscussionId,std::string> _discussionNames;
    std::map<DiscussionId,Address> _discussionSlaves;
    std::map<DiscussionListVersion,std::vector<DiscussionId>>
            _discussionListVersionChangeset;
    DiscussionId _nextDiscussionId=1;
    DiscussionListVersion _nextDiscussionListVersion=2;
    std::mutex _bigDatabaseLock;

    std::vector<Address> _slaves;
    std::mutex _slavesLock;

};

extern Database database;

#endif // DATABASE_H