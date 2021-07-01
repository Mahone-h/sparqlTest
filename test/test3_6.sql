PREFIX foaf:    <http://xmlns.com/foaf/0.1/>
SELECT ?x ?z ?y ?nickY ?g ?h
    WHERE
  { ?x foaf:knows ?z .
    ?z foaf:name ?nameX .
    ?y foaf:nick ?nickY .
    ?g foaf:cro "s5" .
    ?h foaf:cro "s6" .
  }


/*------------------------------------------
| x   | z   | y   | nickY    | g   | h   |
==========================================
| <b> | <c> | <d> | "CTDDDD" | <i> | <k> |
| <b> | <c> | <c> | "CT"     | <i> | <k> |
| <a> | <d> | <d> | "CTDDDD" | <i> | <k> |
| <a> | <d> | <c> | "CT"     | <i> | <k> |
| <a> | <b> | <d> | "CTDDDD" | <i> | <k> |
| <a> | <b> | <c> | "CT"     | <i> | <k> |
| <b> | <c> | <d> | "CTDDDD" | <i> | <l> |
| <b> | <c> | <c> | "CT"     | <i> | <l> |
| <a> | <d> | <d> | "CTDDDD" | <i> | <l> |
| <a> | <d> | <c> | "CT"     | <i> | <l> |
| <a> | <b> | <d> | "CTDDDD" | <i> | <l> |
| <a> | <b> | <c> | "CT"     | <i> | <l> |
| <b> | <c> | <d> | "CTDDDD" | <j> | <k> |
| <b> | <c> | <c> | "CT"     | <j> | <k> |
| <a> | <d> | <d> | "CTDDDD" | <j> | <k> |
| <a> | <d> | <c> | "CT"     | <j> | <k> |
| <a> | <b> | <d> | "CTDDDD" | <j> | <k> |
| <a> | <b> | <c> | "CT"     | <j> | <k> |
| <b> | <c> | <d> | "CTDDDD" | <j> | <l> |
| <b> | <c> | <c> | "CT"     | <j> | <l> |
| <a> | <d> | <d> | "CTDDDD" | <j> | <l> |
| <a> | <d> | <c> | "CT"     | <j> | <l> |
| <a> | <b> | <d> | "CTDDDD" | <j> | <l> |
| <a> | <b> | <c> | "CT"     | <j> | <l> |
------------------------------------------*/



