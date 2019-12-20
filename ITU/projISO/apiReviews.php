<?php

header('Content-type:application/json');
$myFile = "commentsAPI.json";

function insertReview($last_index_value, $myFile)
{
if (array_key_exists('title', $_POST) && array_key_exists('review_text', $_POST))
{
//  $file = file_get_contents($myFile);
//  $data = json_decode($file, true);

// Get last id
//$last_item    = end($data);
//$last_item_id = $last_item['id'];
//echo $last_item_id;
   $title = $_POST['title'];
   $review_text = $_POST['review_text'];
   $value = $last_index_value + 1;

   //echo 'Yes, it works!';
   $array = array(
     "id" => $value,
     "title" => $title,
     "review" => $review_text
   );
   //print_r($array);

   $jsondata =  json_encode($array);
   file_put_contents($myFile, $jsondata.PHP_EOL, FILE_APPEND);
 }

  // $id++;

   //echo $jsondata;

 }

 $indexes_array = array();

 $fn = fopen($myFile,"r");

  while(! feof($fn))  {
	$result = fgets($fn);
  if ($result == "")
  {
    continue;
  }
  #echo $result;
  $obj = json_decode($result);
  array_push($indexes_array, $obj->{'id'}); // 12345
	//echo $result;
  }

  fclose($fn);
  #print_r($indexes_array);
  $last_index_position = count($indexes_array);
  $last_index_value = $indexes_array[$last_index_position - 1];
  #echo $last_index_value;
  insertReview($last_index_value, $myFile);

  $loaded_file = file_get_contents($myFile, true);
  echo $loaded_file;


 //$data = json_decode($loaded_file, true);
 //echo $data;

 // Get last id
 //$last_item = end($data);
 //$last_item_id = $last_item['id'];
 //echo $last_item_id;








?>
