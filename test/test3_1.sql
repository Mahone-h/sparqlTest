PREFIX foaf:    <http://xmlns.com/foaf/0.1/>
SELECT ?nameX ?nameY ?nickY
    WHERE
  { ?x foaf:knows ?y ;
       foaf:name ?nameX .
    ?y foaf:name ?nameY .
    ?y foaf:nick ?nickY .
  }



/*测内部join*/
-- ?nameX	?nameY	?nickY
-- "Alice"	"Clare"	"CT"
-- "Bob"    "Clare" "CT"
