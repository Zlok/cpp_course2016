#include <iostream>

using namespace std;

class list {
public:
  list & push_front(std::string NewData)
  {
    ListStart = new entry(NewData, ListStart); 
    return *this;
  } 
  list & push_back(std::string NewData)
  {
    entry **p = &ListStart;
    while (*p != NULL)
      p = &(*p)->Next;
    *p = new entry(NewData, NULL);
    return *this;
  }
  bool pop_front(std::string *OldData)
  {
    if (ListStart == NULL)
      return false;
    entry *p = ListStart;
    ListStart = ListStart->Next;
    if (OldData != NULL)
      *OldData = p->Data;
    delete p;
    return true;
  }
  bool pop_back(std::string *OldData)
  {
    if (ListStart == NULL)
      return false;
    entry **p = &ListStart;
    while ((*p)->Next != NULL)
      p = &(*p)->Next;
    *OldData = (*p)->Data;  
    delete *p;
    *p = NULL;
    return true;
  } 
  void Display(void) const
  {
    if (ListStart == NULL)
      cout << "Empty list\n";
    else {
      entry *L = ListStart;
      while (L != NULL) {
        cout << L->Data << (L->Next == NULL ? "\n" : ", ");
        L = L->Next;
      }
    }
  }
  list(void) : ListStart(NULL)
  {
  }
  ~list(void)
  {
    std::string Old;
    while(pop_back(&Old))
      ;
  }
private:
  class entry
  {
  public:
    std::string Data;
    entry *Next;
    entry(std::string &NewData, entry *NewNext) : Data(NewData), Next(NewNext)
    {
    }
  } *ListStart;
};
