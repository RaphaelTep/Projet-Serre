<?php
session_start();
include 'db_connect.php';

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $seuil_hum_min_plante1 = $_POST['seuil_hum_min_plante1'] ?? null;
    $seuil_hum_min_plante2 = $_POST['seuil_hum_min_plante2'] ?? null;
    $seuil_hygro_min = $_POST['seuil_hygro_min'] ?? null;
    $seuil_hygro_max = $_POST['seuil_hygro_max'] ?? null;
    $seuil_temp_min = $_POST['seuil_temp_min'] ?? null;
    $seuil_temp_max = $_POST['seuil_temp_max'] ?? null;

    try {
        $stmt = $pdo->prepare("
            UPDATE Seuils SET 
                seuil_hum_min_plante1 = ?,
                seuil_hum_min_plante2 = ?,
                seuil_hygro_min = ?,
                seuil_hygro_max = ?,
                seuil_temp_min = ?,
                seuil_temp_max = ?
        ");

        $stmt->execute([
            $seuil_hum_min_plante1,
            $seuil_hum_min_plante2,
            $seuil_hygro_min,
            $seuil_hygro_max,
            $seuil_temp_min,
            $seuil_temp_max
        ]);

        echo "✅ Seuils bien enregistrés.";
    } catch (PDOException $e) {
        echo "❌ Erreur lors de la mise à jour des seuils : " . $e->getMessage();
    }
} else {
    echo "❌ Méthode non autorisée.";
}
