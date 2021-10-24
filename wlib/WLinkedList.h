/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: WLib
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/


#ifndef WLINKEDLIST_H
#define WLINKEDLIST_H


/**
 WLinkedList

 Template class of a linked l, to store linked objects.

 V  = type of the stored value
 VV = type of the view of the stored value (paramter type)

 example:

 WLinkedList <WString, WCSTR> l;

 l.add ("VALUE1");
 l.add ("VALUE2");
 l.add ("VALUE3");

 while (l.hasMoreElements ())
    printf ("%s", l.nextValue ());

 --------------------------------------------------------------------
 Note !

 The linked-list-class is not MT-Safe
 --------------------------------------------------------------------

 plattform: WIN32 / UNIX

*/

#include "wbase.h"


#define WENTRY typename WLinkedList<V,VV>::WEntry*


class WSorter {
public:
	virtual int compare (void* obj1, void* obj2) const = 0;
	virtual bool isSetAutoKeys () const = 0;
	virtual bool isSortByKey () const = 0;
};

template <class V, class VV>
class WLinkedList {
public:

	// entry
	class WEntry {
	public:
		WEntry ()
		{
			m_last = NULL;
			m_next = NULL;
		}
		V       m_value; // value
		WEntry* m_last;  // link to last entry
		WEntry* m_next;  // link to next entry
	};

	//////////////////////////////////

	WLinkedList ();
	WLinkedList (const WLinkedList<V,VV>& ll)
	{
		initialize ();
		while (ll.hasMoreElements ())
			add (ll.nextValue ());
	};

	virtual ~WLinkedList ();

	WENTRY add             (VV value);
	bool   hasMoreElements () const;
	bool   hasPrevElements () const;
	VV     nextValue       () const;
	VV     prevValue       () const;
	void   remove          (WENTRY entry);
	void   remove          (int cnt);
	void   removeAll       ();
	void   mark            ();
	void   reset           ();
	WENTRY setLast         ();
	W32    size            () const;
	void   sort            (WSorter& sorter);

	WLinkedList<V,VV>& operator = (const WLinkedList<V,VV>& ll)
	{
		removeAll ();
		while (ll.hasMoreElements ())
			add (ll.nextValue ());
		return *this;
	};

protected:

	WEntry*  m_list;        // linked list
	W32      m_size;        // size of the linked list
	WEntry*  m_firstEntry;  // first entry in linked list
	mutable WEntry* m_markEntry;   // marked entry (mark (), reset ())
	mutable WEntry* m_readEntry;   // current entry to read

	// static methods do the work
	static WEntry* add
	(WEntry* l, VV value);

	static void free
	(WEntry* l);

	////////////////////////////////////
	void initialize  ();
};

template <class V, class VV>
void WLinkedList<V,VV>::initialize ()
{
	m_list       = NULL;
	m_readEntry  = NULL;
	m_firstEntry = NULL;
	m_markEntry  = NULL;
	m_size       = 0;
}

template <class V, class VV>
WLinkedList<V,VV>::WLinkedList ()
{
	initialize ();
}

template <class V, class VV>
WLinkedList<V,VV>::~WLinkedList ()
{
	this->free (m_list);
}

template <class V, class VV>
WENTRY WLinkedList<V,VV>::add (WEntry* l, VV value)
{

	WEntry* entry  = new WEntry;

	entry->m_value = value;
	entry->m_next  = l;
	entry->m_last  = NULL;

	if (l!=NULL)
		l->m_last = entry;
	return entry;
}

template <class V, class VV>
WENTRY WLinkedList<V,VV>::add (VV value)
{
	m_list = add (m_list, value);
	if (m_size==0) {
		// save the first entry
		m_firstEntry = m_list;
		m_readEntry  = m_list;
	}
	m_size++;
	return m_list;
}

template <class V, class VV>
void WLinkedList<V,VV>::free (WEntry* l)
{
	WEntry* next;
	while (l!=NULL) {
		next = l->m_next;
		delete l;
		l = next;
	}
}

template <class V, class VV>
bool WLinkedList<V,VV>::hasMoreElements () const
{
	if (m_readEntry!=NULL)
		return TRUE;
	m_readEntry = m_firstEntry;
	return FALSE;
}

template <class V, class VV>
bool WLinkedList<V,VV>::hasPrevElements () const
{
	if (m_readEntry!=NULL)
		return TRUE;
	m_readEntry = m_list;
	return FALSE;
}


template <class V, class VV>
void WLinkedList<V,VV>::mark ()
{
	m_markEntry = m_readEntry;
}


template <class V, class VV>
void WLinkedList<V,VV>::reset ()
{
	if (m_markEntry!=NULL)
		m_readEntry = m_markEntry;
	else
		m_readEntry = m_firstEntry;
}

template <class V, class VV>
VV WLinkedList<V,VV>::nextValue () const
{
	WEntry* entry = m_readEntry;
	m_readEntry = m_readEntry->m_last;
	return entry->m_value;
}


template <class V, class VV>
VV WLinkedList<V,VV>::prevValue () const
{
	WEntry* entry = m_readEntry;
	m_readEntry = m_readEntry->m_next;
	return entry->m_value;
}


template <class V, class VV>
W32 WLinkedList<V,VV>::size () const
{
	return m_size;
}

template <class V, class VV>
void WLinkedList<V,VV>::remove (WENTRY entry)
{
	WEntry* next = entry->m_next;
	WEntry* last = entry->m_last;

	if (next!=NULL)
		next->m_last = last;

	if (last!=NULL)
		last->m_next = next;

	if (entry==m_firstEntry)
		m_firstEntry=last;

	if (entry==m_readEntry)
		m_readEntry=last;

	if (entry==m_markEntry)
		m_markEntry=last;

	if (entry==m_list)
		m_list=next;

	m_size--;
	delete entry;
}

template <class V, class VV>
void WLinkedList<V,VV>::remove (int cnt)
{
	WEntry* entry = m_firstEntry;
	WEntry* last  = NULL;

	while ((entry!=NULL) && (cnt>0)) {
		last  = entry->m_last;

		delete entry;

		if (last!=NULL)
			last->m_next = NULL;

		if (entry==m_readEntry)
			m_readEntry=last;

		if (entry==m_markEntry)
			m_markEntry=last;

		entry = last;
		cnt--;
		m_size--;
	}
	if (entry==NULL)
		m_list = NULL;
	m_firstEntry = entry;
}

template <class V, class VV>
void WLinkedList<V,VV>::removeAll ()
{
	this->free (m_list);
	initialize ();
}

template <class V, class VV>
WENTRY WLinkedList<V,VV>::setLast ()
{
	m_readEntry = m_list;
	return m_readEntry;
}

template <class V, class VV>
void WLinkedList<V,VV>::sort (WSorter& sorter)
{
	W32 swapCnt = 0;
	do {
		swapCnt = 0;
		WEntry* entry = m_firstEntry;
		while (entry!=NULL) {
			WEntry* entry2 = entry->m_last;
			if (entry2!=NULL) {

				int ret = sorter.compare (&entry->m_value, &entry2->m_value);
				if (ret<0) {
					/*
					// Change values
					V tmp;
					tmp = entry2->m_value;
					entry2->m_value = entry->m_value;
					entry->m_value = tmp;
					*/

					// Change links
					if (entry->m_next)
						entry->m_next->m_last = entry2;
					else
						m_firstEntry = entry2;
					if (entry2->m_last)
						entry2->m_last->m_next = entry;
					entry2->m_next = entry->m_next;
					WEntry* tmp = entry2->m_last;
					entry2->m_last = entry;
					entry->m_last = tmp;
					entry->m_next = entry2;
					if (!tmp)
						m_list = entry;
					entry2 = entry;
					swapCnt++;
				}
			}
			entry = entry2;
		}

	}
	while (swapCnt>0);
}

class WString;
typedef WLinkedList<WString, WCSTR> WStringList;

#endif /* WLINKEDLIST_H */