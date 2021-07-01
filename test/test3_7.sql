PREFIX foaf:    <http://xmlns.com/foaf/0.1/>
SELECT ?x   ?z ?y ?nickY
    WHERE
  { ?x foaf:knows ?z .
    ?z foaf:name ?nameX .
    ?y foaf:nick ?nickY .
    ?nickY foaf:name1 ?g.
  }


/*x和z 有path y和nickY有path  nickY 空集*/