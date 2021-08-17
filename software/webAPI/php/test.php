<?php
    $state = $_POST["example"];
    echo "post example:";
    echo $state;
    if(!$FP = fopen("../log/log.csv","a"))
        echo "fopen error";
    else{
        $add_data = array("and test","or test","not test");
        if(!fputcsv($FP,$add_data))
            echo "<br>書込に失敗しました";
        else
            echo "<br>データを書込ました";
        fclose($FP);
    }
?>
