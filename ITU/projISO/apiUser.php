<?php

// Autor: Katerina Fortova (xforto00)

header('Content-type:application/json');
$file = "dataUser.json";

// function for insert new changes for user to json
function insertChangesUser($file)
{
  if (array_key_exists('user', $_POST) && array_key_exists('firstname', $_POST) && array_key_exists('lastname', $_POST) && array_key_exists('favouriteGenres', $_POST) && array_key_exists('interests', $_POST))
  {

    $user = $_POST['user'];
    $firstname = $_POST['firstname'];
    $lastname = $_POST['lastname'];
    $favouriteGenres = $_POST['favouriteGenres'];
    $interests = $_POST['interests'];

    $array = array(
      "user" => $user,
      "firstname" => $firstname,
      "lastname" => $lastname,
      "favouriteGenres" => $favouriteGenres,
      "interests" => $interests,
    );


    $jsondata =  json_encode($array);
    file_put_contents($file, $jsondata.PHP_EOL);
 }


 }


  insertChangesUser($file);

  $loaded_file = file_get_contents($file, true);
  echo $loaded_file;
