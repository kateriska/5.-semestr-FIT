<?php

function show_my_courses_garant($user_id, $db)
{
  $query = "SELECT * FROM kurzy, uzivatele where kurzy.garant_ID = '$user_id' and uzivatele.Uzivatele_ID = '$user_id' and uzivatele.role > 2";
  $result = mysqli_query($db, $query);



  if ($result->num_rows > 0) {
    // output data of each row
    
    while($row = $result->fetch_assoc())
    {
        echo "Kurzy_ID: " . $row["Kurzy_ID"]. " - nazev: " . $row["nazev"]. " " . $row["typ"]. "<br>";
    }

  }
  else
  {
    echo "0 results";
  }
}

require_once("dbh.php");
show_my_courses_garant(9, $db);

?>
