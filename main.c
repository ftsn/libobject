#include <stdio.h>
#include "strings.h"
#include "lists.h"

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
  String       	*converted;

  list = new(_spl_clist, NULL, 4, "Lunkwill", "Frazou", "Roo", "Woklada");
  converted = ((Container *)list)->sub(list, _string, -2, 3);
  ((Container *)list)->dump(list, "Basic list", _list_basic_print, "");
  ((Container *)converted)->dump(converted, "Converted string", _string_basic_print, "");

  return (1);
}
