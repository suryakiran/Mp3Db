declare variable $dir external;

for $x in doc('ConfigQueries.xml')/Queries/Query 
return (
    "[",
    {data($x/@name)}, "-",
    {data($x)},
    "]"
    )
