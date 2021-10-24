/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#ifndef WHASHTABLE_H
#define WHASHTABLE_H

#include "WLinkedList.h"

#define WHASH typename WHashtable<K,V,KV,VV>::WHash*

/**
 Template class of a hashtable.

 K  = type of the stored key
 KV = type of the view of the stored key (paramter type)
 V  = type of the stored value
 VV = type of the view of the stored value (paramter type)

 example:

 WHashtable <WString, WString , WCSTR, WCSTR> ht;

 ht.put ("KEY1", "KEY2");
 ht.get ("KEY1");

 --------------------------------------------------------------------
 Note !

 The hashtable-class is not MT-Safe.
 --------------------------------------------------------------------

 plattform: WIN32 / UNIX

*/

template <class K, class V, class KV, class VV>
class WHashtable {
public:

	enum {
		HASHTABLE_SIZE = 10
	};

	WHashtable ();

	WHashtable (const WHashtable<K,V,KV,VV>& ht)
	{
		initialize ();
		while (ht.hasMoreElements ())
			put (ht.nextKey (), ht.nextValue ());
		m_maxAutoIdx = ht.getMaxAutoIndex ();
		m_fOwnKeys = ht.hasOwnKeys ();
	};

	virtual ~WHashtable ();

	// hash class
	class WHash {
	public:

		WHash ()
		{
			m_hash  = 0;
			m_last  = NULL;
			m_next  = NULL;
		}

		WHash (W16 hash, KV key)
		{
			m_hash  = hash;
			m_key   = key;
			m_next  = NULL;
			m_last  = NULL;
		}

		WHash (W16 hash, KV key, VV value)
		{
			m_hash  = hash;
			m_key   = key;
			m_value = value;
			m_next  = NULL;
			m_last  = NULL;
		}
		W16     m_hash;      // hash value
		K       m_key;       // key
		V       m_value;     // value
		WHash*  m_last;      // link to last hash
		WHash*  m_next;      // link to next hash

		// entry of linked list
		typename WLinkedList<WHash*, WHash*>::WEntry* m_listEntry;
	};


	// get / put methods
	WHash* put               (KV  key);
	WHash* put               (KV  key, VV value);
	VV     get               (KV  key) const;
	V*     getPnt            (KV  key, WHash** hashPnt=NULL) const;

	// push / pop functions
	WHash* push              (VV value);
	bool   pop               (V& value);

	// check key
	bool   isKey             (KV  key) const;
	// remove
	bool   remove            (KV  key);
	void   removeByHash      (WHash* hash);
	void   removeAll         ();
	// read keys / values
	bool   hasMoreElements   () const;
	bool   hasPrevElements   () const;
	VV     nextValue         () const;
	VV     prevValue         () const;
	V*     nextValuePnt      (WHash** hashPnt=NULL) const;
	KV     nextKey           () const;
	void   enumeration       (WLinkedList <WHash, WHash&>& hashList) const;
	void   mark              ();
	void   reset             ();
	void   setLast           ();
	void   sort              (WSorter& sorter);

	// size functions
	W32    size              () const;
	void   setMaxAutoIndex   (int idx);
	int    getMaxAutoIndex   () const;


	// Hashtale has own Keys (only status var)
	void   setOwnKeys        (bool on);
	bool   hasOwnKeys        () const;

	// Copy hashtable
	WHashtable<K,V,KV,VV>& operator=(const WHashtable<K,V,KV,VV>& ht)
	{
		removeAll ();
		while (ht.hasMoreElements ())
			put (ht.nextKey (), ht.nextValue ());
		m_maxAutoIdx = ht.getMaxAutoIndex ();
		m_fOwnKeys = ht.hasOwnKeys ();
		return *this;
	};

protected:

	// hash link
	class WHashLink {
	public:
		inline WHashLink ()
		{
			m_hash = NULL;
		}
		// pointer to first hash in the linked hash-list
		WHash* m_hash;
	};

	// hash-table
	WHashLink* m_hashTable;

	// linked list with hashs to save the right order
	WLinkedList <WHash*, WHash*> m_hashList;

	// Max auto index
	W32 m_maxAutoIdx;

	// Hashtable with own keys
	bool m_fOwnKeys;

	// current hash to read
	mutable WHash* m_currHash;

	// initialize the hashtable
	void initialize ();

	// static methods do the work
	static WHash* put
	(KV key, WHashLink* hashTable, bool &fExist);

	static WHash* put
	(KV key, VV value, WHashLink* hashTable, bool &fExist);

	static WHash* get
	(KV key, WHashLink* hashTable);

	static void remove
	(WHash* hash, WHashLink* hashTable);

	static void free
	(WHashLink* hashTable);

	// hash functions
	////////////////////////////////////////
	static W16 createHash (WCSTR key);
	static W16 createHash (int key);
	static W16 createHash (void* key);
};

template <class K, class V, class KV, class VV>
void WHashtable<K,V,KV,VV>::initialize ()
{
	m_hashTable  = NULL; // Save Memory, no initial size
	m_currHash   = NULL;
	m_maxAutoIdx = -1;
	m_fOwnKeys   = false;
}

template <class K, class V, class KV, class VV>
WHashtable<K,V,KV,VV>::WHashtable ()
{
	initialize ();
}

template <class K, class V, class KV, class VV>
WHashtable<K,V,KV,VV>::~WHashtable ()
{
	if (m_hashTable!=NULL) {
		this->free (m_hashTable);
		m_hashTable = NULL;
	}
}

template <class K, class V, class KV, class VV>
WHASH WHashtable<K,V,KV,VV>::put (KV key, VV value,
                                  WHashLink* hashTable, bool& fExist)
{
	W16 h;
	WHash* hash;

	// check if key is available
	hash = get (key, hashTable);
	if (hash!=NULL) {
		hash->m_value = value;
		fExist = true;
		return hash;
	}


	// create a new hash
	h     = createHash (key);
	hash  = new WHash (h, key, value);
	// link hash
	WHashLink* link = &hashTable[h % HASHTABLE_SIZE];
	hash->m_next = link->m_hash;
	if (link->m_hash!=NULL)
		link->m_hash->m_last = hash;
	link->m_hash = hash;
	fExist = false;
	return hash;
}

template <class K, class V, class KV, class VV>
WHASH WHashtable<K,V,KV,VV>::put (KV key,
                                  WHashLink* hashTable, bool& fExist)
{
	W16 h;
	WHash* hash;

	// check if key is available
	hash = get (key, hashTable);
	if (hash!=NULL) {
		fExist = true;
		return hash;
	}


	// create a new hash
	h     = createHash (key);
	hash  = new WHash (h, key);
	// link hash
	WHashLink* link = &hashTable[h % HASHTABLE_SIZE];
	hash->m_next = link->m_hash;
	if (link->m_hash!=NULL)
		link->m_hash->m_last = hash;
	link->m_hash = hash;
	fExist = false;
	return hash;
}




template <class K, class V, class KV, class VV>
WHASH WHashtable<K,V, KV, VV>::get (KV key, WHashLink* hashTable)
{
	if (!hashTable)
		return NULL;

	// calculate hash
	W16 h = createHash (key);
	// search hash-object
	WHashLink* link = &hashTable[h % HASHTABLE_SIZE];
	WHash* hash;
	hash = link->m_hash;
	while (hash!=NULL) {
		if (hash->m_hash==h) {
			// verify the key
			if (hash->m_key==key)
				return hash;
		}
		hash = hash->m_next;
	}
	// no hash-object found
	return NULL;
}


template <class K, class V, class KV, class VV>
void WHashtable<K,V, KV, VV>::remove (WHash* hash, WHashLink* hashTable)
{
	if (!hashTable)
		return;

	WHash* next = hash->m_next;
	WHash* last = hash->m_last;
	if (next!=NULL)
		next->m_last = last;
	if (last!=NULL)
		last->m_next = next;
	WHashLink* link  = &hashTable[hash->m_hash % HASHTABLE_SIZE];
	if (link->m_hash ==hash)
		link->m_hash = next;
	delete hash;
}

template <class K, class V, class KV, class VV>
bool WHashtable<K,V,KV,VV>::remove (KV key)
{
	if (!m_hashTable)
		return false;

	WHash* hash = get (key, m_hashTable);
	if (hash!=NULL) {
		m_hashList.remove (hash->m_listEntry);
		remove (hash, m_hashTable);
		return TRUE;
	}
	return FALSE;
}

template <class K, class V, class KV, class VV>
void WHashtable<K,V,KV,VV>::removeByHash (WHASH hash)
{
	if (!m_hashTable)
		return;

	m_hashList.remove (hash->m_listEntry);
	remove (hash, m_hashTable);
}

template <class K, class V, class KV, class VV>
void WHashtable<K,V,KV,VV>::removeAll ()
{
	if (m_hashTable!=NULL) {
		this->free (m_hashTable);
		m_hashTable=NULL;
		m_hashList.removeAll ();
		initialize ();
	}
}

template <class K, class V, class KV, class VV>
bool WHashtable<K,V,KV,VV>::isKey (KV key) const
{
	WHash* hash = get (key, m_hashTable);
	if (hash!=NULL)
		return TRUE;
	return FALSE;
}

template <class K, class V, class KV, class VV>
VV WHashtable<K,V,KV,VV>::get (KV key) const
{
	WHash* hash = get (key, m_hashTable);
	return hash->m_value;
}

template <class K, class V, class KV, class VV>
V* WHashtable<K,V,KV,VV>::getPnt (KV key, WHash** hashPnt) const
{
	WHash* hash = get (key, m_hashTable);
	if (hashPnt!= NULL)
		*hashPnt = hash;
	if (hash==NULL)
		return NULL;
	return &hash->m_value;
}

template <class K, class V, class KV, class VV>
bool WHashtable<K,V,KV,VV>::pop (V& value)
{
	typename WLinkedList<WHash*, WHash*>::WEntry* entry = m_hashList.setLast ();
	if (entry) {
		WHash* hash = entry->m_value;
		value = hash->m_value;
		m_hashList.remove (entry);
		remove (hash, m_hashTable);
		return true;
	}
	return false;
}

template <class K, class V, class KV, class VV>
WHASH WHashtable<K,V,KV,VV>::put (KV key, VV value)
{
	if (!m_hashTable)
		m_hashTable = new WHashLink [HASHTABLE_SIZE];

	bool fExists;
	WHash* hash = put (key, value, m_hashTable, fExists);
	if (!fExists)
		hash->m_listEntry = m_hashList.add (hash);
	return hash;
}

template <class K, class V, class KV, class VV>
WHASH WHashtable<K,V,KV,VV>::put (KV key)
{
	if (!m_hashTable)
		m_hashTable = new WHashLink [HASHTABLE_SIZE];

	bool fExists;
	WHash* hash = put (key, m_hashTable, fExists);
	if (!fExists)
		hash->m_listEntry = m_hashList.add (hash);
	return hash;
}


template <class K, class V, class KV, class VV>
WHASH WHashtable<K,V,KV,VV>::push (VV value)
{
	if (!m_hashTable)
		m_hashTable = new WHashLink [HASHTABLE_SIZE];

	m_maxAutoIdx++;
	bool fExists;
	WHash* hash = put (WFormattedString("%u", (W32) m_maxAutoIdx), value, m_hashTable, fExists);
	if (!fExists)
		hash->m_listEntry = m_hashList.add (hash);
	return hash;
}

template <class K, class V, class KV, class VV>
void WHashtable<K,V,KV, VV>::free (WHashLink* hashTable)
{
	if (!hashTable)
		return;

	WHash* hash;
	WHash* next;
	for (int i=0; i<HASHTABLE_SIZE; i++) {
		hash = hashTable[i].m_hash;
		while (hash!=NULL) {
			next = hash->m_next;
			delete hash;
			hash = next;
		}
		hashTable[i].m_hash = NULL;
	}
	delete[] hashTable;
}

template <class K, class V, class KV, class VV>
bool WHashtable<K,V,KV,VV>::hasMoreElements () const
{
	if (m_hashList.hasMoreElements ()) {
		m_currHash = m_hashList.nextValue ();
		return true;
	}
	return false;
}

template <class K, class V, class KV, class VV>
bool WHashtable<K,V,KV,VV>::hasPrevElements () const
{
	if (m_hashList.hasPrevElements ()) {
		m_currHash = m_hashList.prevValue ();
		return TRUE;
	}
	return FALSE;
}

template <class K, class V, class KV, class VV>
void WHashtable<K,V,KV,VV>::enumeration  (WLinkedList <WHash, WHash&>& hashList) const
{
	hashList.removeAll ();
	if (!m_hashTable)
		return;

	while (m_hashList.hasMoreElements ())
		hashList.add (*m_hashList.nextValue ());
}

template <class K, class V, class KV, class VV>
VV WHashtable<K,V,KV,VV>::nextValue () const
{
	return m_currHash->m_value;
}

template <class K, class V, class KV, class VV>
VV WHashtable<K,V,KV,VV>::prevValue () const
{
	return m_currHash->m_value;
}

template <class K, class V, class KV, class VV>
V* WHashtable<K,V,KV,VV>::nextValuePnt (WHash** hashPnt) const
{
	if (hashPnt!= NULL)
		*hashPnt = m_currHash;
	return &m_currHash->m_value;
}

template <class K, class V, class KV, class VV>
KV WHashtable<K,V,KV,VV>::nextKey () const
{
	return m_currHash->m_key;
}

template <class K, class V, class KV, class VV>
void WHashtable<K,V,KV,VV>::mark ()
{
	m_hashList.mark ();
}

template <class K, class V, class KV, class VV>
void WHashtable<K,V,KV,VV>::reset ()
{
	m_hashList.reset ();
}

template <class K, class V, class KV, class VV>
void WHashtable<K,V,KV,VV>::setLast ()
{
	m_hashList.setLast ();
}


template <class K, class V, class KV, class VV>
W32 WHashtable<K,V,KV,VV>::size () const
{
	return m_hashList.size ();
}

template <class K, class V, class KV, class VV>
void WHashtable<K,V,KV,VV>::setMaxAutoIndex (int idx)
{
	m_maxAutoIdx = idx;
}

template <class K, class V, class KV, class VV>
int WHashtable<K,V,KV,VV>::getMaxAutoIndex () const
{
	return m_maxAutoIdx;
}


template <class K, class V, class KV, class VV>
bool WHashtable<K,V,KV,VV>::hasOwnKeys () const
{
	return m_fOwnKeys;
}


template <class K, class V, class KV, class VV>
void WHashtable<K,V,KV,VV>::setOwnKeys (bool on)
{
	m_fOwnKeys = on;
}

template <class K, class V, class KV, class VV>
W16 WHashtable<K, V, KV, VV>::createHash (WCSTR key)
{
	W32 h = 0;
	while (*key)
		h += *key++;
	return (W16) h;
}

template <class K, class V, class KV, class VV>
W16 WHashtable<K, V, KV, VV>::createHash (int key)
{
	return (W16) (40503 * key) >> 8;
}

template <class K, class V, class KV, class VV>
W16 WHashtable<K, V, KV, VV>::createHash (void* key)
{
	return (W16) (40503 * (uint64_t) key) >> 8;
}

template <class K, class V, class KV, class VV>
void WHashtable<K, V, KV, VV>::sort (WSorter& sorter)
{
	class MySorter : public WSorter {
	public:
		MySorter (WSorter& sorter, WHashLink* hashTable)
		{
			m_sorter = &sorter;
			m_hashTable = hashTable;
		}
		int compare (void* obj1, void* obj2) const
		{
			WHash* hash1 = *(WHash**) obj1;
			WHash* hash2 = *(WHash**) obj2;
			int ret;
			if (m_sorter->isSortByKey ())
				ret =  m_sorter->compare (&hash1->m_key, &hash2->m_key);
			else
				ret =  m_sorter->compare (&hash1->m_value, &hash2->m_value);
			return ret;
		}
		bool isSetAutoKeys () const
		{
			return m_sorter->isSetAutoKeys ();
		}
		bool isSortByKey () const
		{
			return m_sorter->isSortByKey ();
		}
	private:
		WSorter* m_sorter;
		WHashLink* m_hashTable;
	};
	MySorter mySorter (sorter, m_hashTable);
	m_hashList.sort (mySorter);

	if (sorter.isSetAutoKeys ()) {
		// Set auto keys
		if (m_hashTable!=NULL) {
			delete[] m_hashTable;
		}
		m_hashTable = new WHashLink [HASHTABLE_SIZE];

		m_hashList.reset ();
		int idx = 0;
		while (m_hashList.hasMoreElements ()) {
			WHash* hash = m_hashList.nextValue ();

			// Create new key
			hash->m_key  = WFormattedString ("%u", idx);
			hash->m_hash = createHash (hash->m_key);
			WHashLink* link = &m_hashTable[hash->m_hash % HASHTABLE_SIZE];
			hash->m_next = link->m_hash;
			if (link->m_hash!=NULL)
				link->m_hash->m_last = hash;
			link->m_hash = hash;

			m_maxAutoIdx = idx++;
		}
	}

}

class WString;
typedef WHashtable<WString,WString,WCSTR,WCSTR> WKeyValueHT;

#endif /* WHASHTABLE_H */
