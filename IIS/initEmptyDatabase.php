<?php
  $db = mysqli_init();
  $login = $argv[1];
  $password = $argv[2];
  if (!mysqli_real_connect($db, 'localhost', $login, $password, $login, 0, '/var/run/mysql/mysql.sock'))
  {
    die('cannot connect '.mysqli_connecterror());
  }

  $user = "CREATE TABLE uzivatel (
  Uzivatel_ID INT NOT NULL AUTO_INCREMENT,
  jmeno VARCHAR(25) NOT NULL,
  prijmeni VARCHAR(25) NOT NULL,
  heslo VARCHAR(30) NOT NULL,
  role INT NOT NULL,
  email VARCHAR(25) NOT NULL,
  PRIMARY KEY (Uzivatel_ID)
)";

if(mysqli_query($db, $user)){
    echo "Table created successfully.";
} else{
    echo "ERROR: Could not able to execute $user. " . mysqli_error($link);
}
?>
