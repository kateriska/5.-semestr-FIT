<?php

header('Content-type:application/json');
$file = "dataLikes.json";

function insertLike($file)
{
  if (array_key_exists('title', $_POST))
  {

    $title = $_POST['title'];
    $like_change = 1;

    $array = array(
      "title" => $title,
      "like" => $like_change
    );


    $jsondata =  json_encode($array);
    file_put_contents($file, $jsondata.PHP_EOL, FILE_APPEND);
 }


 }


  insertLike($file);

  $loaded_file = file_get_contents($file, true);
  echo $loaded_file;









?>
