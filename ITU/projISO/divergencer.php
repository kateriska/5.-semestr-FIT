<!-- Autor: Katerina Fortova (xforto00) -->
<html>
<?php
   /* Defining a PHP Function */
   function writeReviewJSON($title, $review_text)
   {
     $myFile = "commentsAPI.json";

      echo "You are really a nice person, Have a nice time!";
      $array = array(
    "title" => $title,
    "review" => $review_text
);
    print_r($array);

    $jsondata =  json_encode($array);

    if(file_put_contents($myFile, $jsondata, FILE_APPEND)) {
	        echo 'Data successfully saved';
	    }
	   else
	        echo "error";



    }

    function readReviewJSON()
    {

    }

    if (isset($_POST['post_review_button']) && isset($_POST['review']) )
    {
      $review_text = $_POST['review'];

      writeReviewJSON("Divergence", $review_text);
    }




   /* Calling a PHP Function */


?>
<script>
var dislikes = 0;
// fuction for counting dislikes of book
function dislikeCounter()
{
    dislikes += 1;
    document.getElementById("dislikes").innerHTML = dislikes;
};
</script>
<script>
var likes = 2;
// fuction for counting likes of book
function likeCounter()
{
    likes += 1;
    document.getElementById("likes").innerHTML = likes;
};
</script>
<script>
// fuction for adding a comment
function addReview() {
  var parent = document.getElementById('profileimage');
  var review = document.forms["submitForm"]["review"].value;
  if (review == "")
  {
    alert("Nesm�te zadat pr�zdnou recenzi!");
    return false;
  }
  var new_child = '<p><h4> <a href="katerina_fortovar.html">Kate�ina Fo�tov�</a></h4></p><p>'+ review + '</p><br>';
  parent.insertAdjacentHTML('beforeend', new_child);
  var all_reviews = document.getElementById('profileimage');
  writeReviewJSON(all_reviews);
  all_reviews.scrollTop = all_reviews.scrollHeight;
/*
  const fs = require('fs');

// Data which will write in a file.
let data = "Learning how to write in a file."

// Write data in 'Output.txt' .
fs.writeFile('Output.txt', data, (err) => {

    // In case of a error throw err.
    if (err)
    {
      alert("Error pri zapisu do souboru");
    }
})
*/
}
</script>
  <head>
    <meta charset="iso-8859-2">
    <meta name="viewport" content="width=device-width">
    <link rel="stylesheet" href="./css/style.css">
  </head>
  <body>

    <header>
      <div class="container">
        <div id="branding">
          <h1><span class="highlight">�ten��sk� klub Loreta</h1>
        </div>
        <nav>
          <ul>
            <li><a href="index.html">Odhl�sit u�ivatele</a></li>
            <li class="current"><a href="book_listr.html">Katalog knih</a></li>
            <li><a href="katerina_fortovar.html">M�j profil</a></li>
          </ul>
        </nav>
      </div>
    </header>

    <section id="formstyle">
      <div class="container">
        <h1>Divergence</h1>
        <form>
              <button onclick="dislikeCounter()" type="button" class="button_1">Nel�bilo se</button>
        </form>
        <form>
              <button  onclick="likeCounter()" type="button" class="button_1">L�bilo se</button>
        </form>

      </div>
    </section>

    <section id="main">
  <div class="container">
    <div id="bookimage">
    <p><img src="./img/divergence.jpg" alt="Divergence" style="width:230px;margin-right:15px;">
      <p><b>Autor: </b>Veronica Roth</p>
      <p><b>��nr: </b>Dystopie</p>
      <p><b>Hodnocen�: </b>L�bilo se: <a id="likes">2</a>  Nel�bilo se: <a id="dislikes">0</a></p>
      <p> Hlavn� hrdinka Beatrice �ije v Chicagu budoucnosti, kde je spole�nost rozd�lena do 5 frakc� a ka�d� z nich se v�nuje p�stov�n� ur�it� ctnosti - Odevzdanost, Neohro�enost, Se�t�lost, M�rumilovnost a Up��mnost. Ka�d� rok ve stanoven� den si mus� v�ichni �estn�ctilet� vybrat jednu z frakc�, do n� budou pat�it po cel� sv�j �ivot. Beatrice se rozhoduje mezi t�m, jestli z�stane s rodinou, nebo bude sama sebou - oboj� m�t nem��e...</p>
    </p>
    <p><a href="https://www.knihydobrovsky.cz/divergence-83519"><b>Koupit knihu online</b></a></p>
        </div>
    </section>

    <section id="main">
  <div class="container">
    <div id="bookimage">
      <select>
        <option value="read">P�e�teno</option>
        <option value="toread">Chci si p�e��st</option>
        <option value="toread">Odebrat z seznamu knih</option>
      </select>
        </div>
    </section>



    <section id="main">
  <div class="container">
 <p><i><h3>Divergence pat�� pr�vem k nej�ten�j��m knih�m sou�asnosti. P��b�h je promy�len do nejmen��ch detail� a je pln� p�ekvapen�, tak�e �ten�� nem� nejmen�� �anci se nudit. Knihu doporu�uji v�em, kte�� stejn� jako j� miluj� p��b�hy z budoucnosti rozv�jej�c� fantazii.</i></h3> - <a href="http://www.klubknihomolu.cz/17963/mila-beatrice-jsi-divergentni-ale-nikomu-to-nerikej/">klubknihomolu.cz</a></p>
  </div>
</section>

  <br>


    <section id="main">
  <div class="container">
    <div id="profileimage">
      <p><h4> <a href="katerina_fortovar.html">Kate�ina Fo�tov�</a></h4></p>
      <p>Jedna z nejlep��ch dystopi�. A�koliv dal�� d�ly u� jsou o trochu slab��, prvn� je z�rukou kvality.</p>
    </p>
    <br>
    <p><h4> <a href="tereza_cernar.html">Tereza �ern�</a></h4></p>
    <p>Svi�n� napsan� p��b�h nadupan� symboly a metaforami, kter� maj� velkou hloubku. Pokud se v�m chce p�em��let, najdete tu spoustu t�mat a pokud ne, u�ijete si "jen" p�kn� ak�n� �ten�.
V�ele doporu�uji.</p>
  <br>
        </div>
      </div>
    </section>

    <section id="main">
  <div class="container">
    <div id="sendreview">
    <form name="submitForm" method="post" action="divergencer.php" >
      <br><textarea name="review" placeholder="Sem zadejte recenzi..."></textarea><br>
      <input type="submit" name="post_review_button"  class="button_3" onclick="addReview()" value="Odeslat">
    </form>
    </div>
  </div>
</section>


    <footer>
      <p>�ten��sk� klub Loreta 2019</p>
    </footer>
  </body>
</html>
