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
    alert("Nesmíte zadat prázdnou recenzi!");
    return false;
  }
  var new_child = '<p><h4> <a href="katerina_fortovar.html">Kateøina Foøtová</a></h4></p><p>'+ review + '</p><br>';
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
          <h1><span class="highlight">Ètenáøský klub Loreta</h1>
        </div>
        <nav>
          <ul>
            <li><a href="index.html">Odhlásit u¾ivatele</a></li>
            <li class="current"><a href="book_listr.html">Katalog knih</a></li>
            <li><a href="katerina_fortovar.html">Mùj profil</a></li>
          </ul>
        </nav>
      </div>
    </header>

    <section id="formstyle">
      <div class="container">
        <h1>Divergence</h1>
        <form>
              <button onclick="dislikeCounter()" type="button" class="button_1">Nelíbilo se</button>
        </form>
        <form>
              <button  onclick="likeCounter()" type="button" class="button_1">Líbilo se</button>
        </form>

      </div>
    </section>

    <section id="main">
  <div class="container">
    <div id="bookimage">
    <p><img src="./img/divergence.jpg" alt="Divergence" style="width:230px;margin-right:15px;">
      <p><b>Autor: </b>Veronica Roth</p>
      <p><b>®ánr: </b>Dystopie</p>
      <p><b>Hodnocení: </b>Líbilo se: <a id="likes">2</a>  Nelíbilo se: <a id="dislikes">0</a></p>
      <p> Hlavní hrdinka Beatrice ¾ije v Chicagu budoucnosti, kde je spoleènost rozdìlena do 5 frakcí­ a ka¾dá z nich se vìnuje pìstování urèité ctnosti - Odevzdanost, Neohro¾enost, Seètìlost, Mí­rumilovnost a Upøímnost. Ka¾dý rok ve stanovený den si musí v¹ichni ¹estnáctiletí­ vybrat jednu z frakcí­, do ní¾ budou patøit po celý svùj ¾ivot. Beatrice se rozhoduje mezi tím, jestli zùstane s rodinou, nebo bude sama sebou - obojí mít nemù¾e...</p>
    </p>
    <p><a href="https://www.knihydobrovsky.cz/divergence-83519"><b>Koupit knihu online</b></a></p>
        </div>
    </section>

    <section id="main">
  <div class="container">
    <div id="bookimage">
      <select>
        <option value="read">Pøeèteno</option>
        <option value="toread">Chci si pøeèíst</option>
        <option value="toread">Odebrat z seznamu knih</option>
      </select>
        </div>
    </section>



    <section id="main">
  <div class="container">
 <p><i><h3>Divergence patøí právem k nejètenìj¹ím knihám souèasnosti. Pøíbìh je promy¹len do nejmen¹ích detailù a je plný pøekvapení, tak¾e ètenáø nemá nejmen¹í ¹anci se nudit. Knihu doporuèuji v¹em, kteøí stejnì jako já milují pøíbìhy z budoucnosti rozvíjející fantazii.</i></h3> - <a href="http://www.klubknihomolu.cz/17963/mila-beatrice-jsi-divergentni-ale-nikomu-to-nerikej/">klubknihomolu.cz</a></p>
  </div>
</section>

  <br>


    <section id="main">
  <div class="container">
    <div id="profileimage">
      <p><h4> <a href="katerina_fortovar.html">Kateøina Foøtová</a></h4></p>
      <p>Jedna z nejlep¹ích dystopií. Aèkoliv dal¹í díly u¾ jsou o trochu slab¹í, první je zárukou kvality.</p>
    </p>
    <br>
    <p><h4> <a href="tereza_cernar.html">Tereza Èerná</a></h4></p>
    <p>Svi¾nì napsaný pøíbìh nadupaný symboly a metaforami, které mají velkou hloubku. Pokud se vám chce pøemý¹let, najdete tu spoustu témat a pokud ne, u¾ijete si "jen" pìkné akèní ètení.
Vøele doporuèuji.</p>
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
      <p>Ètenáøský klub Loreta 2019</p>
    </footer>
  </body>
</html>
