# Sorting in external memory

This program takes as an input a file with urls and their respective number of times these URLs have been visitted:

```
http://www.google.com 3
http://www.uol.com.br 5
http://aeds.dcc.ufmg.br 6
http://www.dcc.ufmg.br 1
http://www.dcc.ufmg.br/cursos 2
```

This file can be so big to the point where it's bigger than our RAM size. Thus, we use external sorting to sort the entries in the input file by the number of visits, in descending order. Ties are broken by alphabetical order.

```
http://aeds.dcc.ufmg.br 6
http://www.uol.com.br 5
http://www.google.com 3
http://www.dcc.ufmg.br/cursos 2
http://www.dcc.ufmg.br 1
```
