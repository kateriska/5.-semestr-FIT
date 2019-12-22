<?php

// Autor: Katerina Fortova (xforto00)

header('Content-type:application/json');
$file = "dataReviews.json";

// function for create new json with new review for book written by user
function insertReview($last_index_value, $file)
{
  if (array_key_exists('title', $_POST) && array_key_exists('review_text', $_POST) && array_key_exists('user', $_POST))
  {

    $title = $_POST['title'];
    $review_text = $_POST['review_text'];
    $user = $_POST['user'];
    $value = $last_index_value + 1;

    $array = array(
      "id" => $value,
      "title" => $title,
      "user" => $user,
      "review" => $review_text
    );


    $jsondata =  json_encode($array);
    file_put_contents($file, $jsondata.PHP_EOL, FILE_APPEND);
 }


 }

 $indexes_array = array();

 $fn = fopen($file,"r");

  while(! feof($fn))
  {
	   $result = fgets($fn);
     if ($result == "")
     {
       continue;
     }

     $obj = json_decode($result);
     array_push($indexes_array, $obj->{'id'});
  }

  fclose($fn);
  $last_index_position = count($indexes_array);
  $last_index_value = $indexes_array[$last_index_position - 1];
  echo($last_index_value);
  insertReview($last_index_value, $file);

  $loaded_file = file_get_contents($file, true);
  echo $loaded_file;









?>
