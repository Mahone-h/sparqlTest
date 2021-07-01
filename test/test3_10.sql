PREFIX foaf:    <http://xmlns.com/foaf/0.1/>
SELECT ?nameX ?nameY ?nickY
    WHERE
  { ?x foaf:knows ?y ;
    ?x foaf:name ?nameX .
    ?y foaf:name ?nameY .
    OPTIONAL { ?y foaf:nick ?nickY }
  }





-- ?nameX	?nameY	?nickY
-- "Alice"	"Bob"
-- "Alice"	"Clare"	"CT"

