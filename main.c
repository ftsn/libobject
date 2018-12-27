#include <stdio.h>
#include "strings.h"
#include "lists.h"
#include "arrays.h"
#include "dicts.h"
#include "iterators.h"

int		main(void)
{
  String	*string;
  String	*splited;

  string = new(_string, "TOTO CHIBRE", 0);
  ((Container *)string)->dump(string, "Title", _string_basic_print, "");

  splited = string->split(string, _string, " \t\n");
  ((Container *)string)->push_back(string, "!");

  ((Container *)string)->dump(string, "Title", _string_basic_print, "");
  ((Container *)splited)->dump(splited, "Title", _string_basic_print, "");
  delete((Object **)&string);
  delete((Object **)&splited);

  List		*list;
  List       	*converted;

  list = new(_spl_clist, NULL, 4, "Lunkwill", "Frazou", "Roo", "Woklada");
  converted = ((Container *)list)->sub(list, _spl_clist, 12, 2);
  ((Container *)list)->dump(list, "Base list", _list_basic_print, "");
  ((Container *)converted)->dump(converted, "Converted list", _list_basic_print, "");
  delete((Object **)&list);
  delete((Object **)&converted);

  Array		*array;
  Array		*converted1;
  Array		*copied;
  List		*converted2;

  array = new(_array, NULL, 4, "Ftsn", "Strelitzia", "E a s y B o t", "Deletebrig");
  converted1 = ((Container *)array)->sub(array, _array, -2, 2);
  copied = new(_array, ((Container *)array)->contained, COPY_ALL, 2, "same start", "but different end");
  converted2 = ((Container *)array)->convert(array, _spl_clist);
  ((Container *)array)->dump(array, "Base array", array_basic_print, "");
  ((Container *)converted1)->dump(converted1, "Converted array", array_basic_print, "");
  ((Container *)copied)->dump(copied, "Copied array", array_basic_print, "");
  ((Container *)converted2)->dump(converted2, "Converted list (from an array)", _list_basic_print, "");
  delete((Object **)&array);
  delete((Object **)&converted1);
  delete((Object **)&copied);
  delete((Object **)&converted2);

  Dict		*dict;
  Array		*converted3;
  
  dict = new(_dict);
  dict->push_back(dict, "MyKey", "MyValue");
  converted3 = ((Container *)dict)->convert(dict, _array);
  ((Container *)dict)->dump(dict, "Dict dump", pair_basic_print, "");
  ((Container *)converted3)->dump(converted3, "Converted array (from a dict)", pair_basic_print, "");
  delete((Object **)&dict);
  delete((Object **)&converted3);

  Dict       	*dict1;
  DictIt	*it;

  dict1 = new(_dict);
  dict1->push_back(dict1, "MyKey", "MyValue");
  dict1->push_back(dict1, "MyKey1", "MyValue1");
  dict1->push_back(dict1, "MyKey2", "MyValue2");
  it = new(_dict_it, dict1);
  while (it->rvalue(it) != NULL)
    {
      printf("key: [%s]\tvalue: [%s]\n", ((t_pair *)it->rvalue(it))->key, ((t_pair *)it->rvalue(it))->data);
      it->incr(it);
    }
  it->decr(it);
  while (it->rvalue(it) != NULL)
    {
      printf("key: [%s]\tvalue: [%s]\n", ((t_pair *)it->rvalue(it))->key, ((t_pair *)it->rvalue(it))->data);
      it->decr(it);
    }
  return (1);
}
