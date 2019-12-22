<?php

// Autor: Katerina Fortova (xforto00)

header('Content-type:application/json');
$file = "dataDislikes.json";

// function for create json with new dislike inserted for specific book
function insertDislike($file)
{
  if (array_key_exists('title', $_POST))
  {

    $title = $_POST['title'];
    $dislike_change = 1;

    $array = array(
      "title" => $title,
      "dislike" => $dislike_change
    );


    $jsondata =  json_encode($array);
    file_put_contents($file, $jsondata.PHP_EOL, FILE_APPEND);
 }


 }


  insertDislike($file);

  $loaded_file = file_get_contents($file, true);
  echo $loaded_file;









?>
