<?php

header('Content-type:application/json');
$file = "dataList.json";

function insertToList($file)
{
  if (array_key_exists('title', $_POST) && array_key_exists('page', $_POST) && array_key_exists('image', $_POST) && array_key_exists('author', $_POST) && array_key_exists('user', $_POST) && array_key_exists('booklist', $_POST))
  {

    $title = $_POST['title'];
    $author = $_POST['author'];
    $booklist = $_POST['booklist'];
    $user = $_POST['user'];
    $image = $_POST['image'];
    $page = $_POST['page'];

    $array = array(
      "title" => $title,
      "page" => $page,
      "author" => $author,
      "image" => $image,
      "user" => $user,
      "booklist" => $booklist
    );


    $jsondata =  json_encode($array);
    file_put_contents($file, $jsondata.PHP_EOL, FILE_APPEND);
 }


 }


  insertToList($file);

  $loaded_file = file_get_contents($file, true);
  echo $loaded_file;









?>
