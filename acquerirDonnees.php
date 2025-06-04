<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);

include 'db_connect.php';

// Requête vers l’ESP32
$json = shell_exec('wget  http://192.168.4.1/mesures');

//echo "igbviygvuygv : " . $json;

$filename = "mesures";
$handle = fopen($filename, "r");
$contents = fread($handle, filesize($filename));
fclose($handle);

// Décoder le JSON
//$data = json_decode($json, true);
$data = json_decode($contents, true);

if ($data === null) {
    die("Erreur : Données invalides ou ESP32 non joignable.");
}

// Extraire les mesures
$hum1 = $data['hum_plante_1'];
$hum2 = $data['hum_plante_2'];
$temp = $data['temperature'];
$hygro = $data['hygrometrie'];

// Enregistrer dans la base
$stmt = $pdo->prepare("
    INSERT INTO Mesures (date_heure, hum_plante_1, hum_plante_2, temperature, hygrometrie)
    VALUES (NOW(), ?, ?, ?, ?)
");
$stmt->execute([$hum1, $hum2, $temp, $hygro]);

echo "Mesures enregistrées avec succès.";
?>
