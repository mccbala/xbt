// server.h: interface for the Cserver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVER_H__B9726CD5_D101_4193_A555_69102FC058E9__INCLUDED_)
#define AFX_SERVER_H__B9726CD5_D101_4193_A555_69102FC058E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sql/database.h"
#include "connection.h"
#include "peer_link.h"
#include "tracker_input.h"
#include "transaction.h"

class Cserver  
{
public:
	struct t_peer
	{
		t_peer()
		{
			listening = false;
			mtime = 0;
		}

		// __int64 downloaded;
		__int64 left;
		string peer_id;
		int port;
		// __int64 uploaded;

		bool listening;
		int mtime;
	};

	typedef map<int, t_peer> t_peers;

	struct t_file
	{
		void clean_up(int t);
		string debug() const;
		Cbvalue select_peers(const Ctracker_input& ti) const;
		Cbvalue scrape() const;

		t_file()
		{
			announced = completed = fid = leechers = scraped = seeders = started = stopped = 0;
			dirty = true;
		}

		t_peers peers;
		int announced;
		int completed;
		bool dirty;
		int fid;
		int leechers;
		int scraped;
		int seeders;
		int started;
		int stopped;
	};

	typedef map<string, t_file> t_files;

	void read_config();
	void write_db();
	void read_db();
	void clean_up();
	void insert_peer(const Ctracker_input&);
	void update_peer(const string& file_id, int peer_id, bool listening);
	string debug(const Ctracker_input&) const;
	Cbvalue select_peers(const Ctracker_input&);
	Cbvalue scrape(const Ctracker_input&);
	void run(Csocket& lt, Csocket& lu);
	Cserver(Cdatabase&);

	int announce_interval() const
	{
		return m_announce_interval;
	}

	const t_files& files() const
	{
		return m_files;
	}

	__int64 secret() const
	{
		return m_secret;
	}
private:
	typedef list<Cconnection> t_connections;
	typedef list<Cpeer_link> t_peer_links;

	int m_clean_up_time;
	int m_read_config_time;
	int m_read_db_time;
	int m_write_db_time;
	int m_announce_interval;
	int m_clean_up_interval;
	int m_read_config_interval;
	int m_read_db_interval;
	int m_write_db_interval;
	int m_fid_end;
	bool m_auto_register;
	bool m_listen_check;
	bool m_log;
	__int64 m_secret;
	t_connections m_connections;
	t_peer_links m_peer_links;
	Cdatabase& m_database;
	t_files m_files;
	string m_announce_log_buffer;
	string m_scrape_log_buffer;
};

#endif // !defined(AFX_SERVER_H__B9726CD5_D101_4193_A555_69102FC058E9__INCLUDED_)
