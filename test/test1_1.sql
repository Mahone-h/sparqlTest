PREFIX foaf:   <http://xmlns.com/foaf/0.1/>
SELECT ?x ?name ?mbox
    WHERE
  { ?x foaf:name ?name .
    ?x foaf:mbox ?mbox .}



--x      name                    mbox
--a      "Johnny Lee Outlaw"	 <mailto:jlow@example.com>
--b      "Peter Goodguy"	     <mailto:peter@example.org>
