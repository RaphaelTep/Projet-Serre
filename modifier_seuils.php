<?php
session_start();
include 'db_connect.php';

if (!isset($_SESSION['connecte']) || $_SESSION['connecte'] !== true) {
    header('Location: login.php');
    exit;
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $humidite = $_POST['seuil_humidite'];
    $temperature = $_POST['seuil_temperature'];

    // Exemple simple : stockage dans une table 'seuils' avec une seule ligne
    $stmt = $pdo->prepare("UPDATE Seuils SET seuil_humidite = ?, seuil_temperature = ? WHERE id = 1");
    $stmt->execute([$humidite, $temperature]);

    echo "✅ Seuils mis à jour avec succès.";
}
?>