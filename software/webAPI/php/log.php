
<HTML>
<HEAD>
<TITLE>log</TITLE>
<?php
    $nowDay = date("Y/m/d");
    $nowTime = date("H:i:s");
    $state = $_POST["state"];
    $ymd = $_POST["ymd"];
    $time = $_POST["time"];
    // $sOrA = $_POST["start_arrival"];
    // echo "post example:";
    // echo $state;
    // echo "<br>";
    if(!$FP = fopen("../log/log.csv","a"))
        echo "fopen error";
    else{
        $arr=array($nowDay,$nowTime,$state,$ymd,$time);
        print_r(array_values($arr));
        if(!fputcsv($FP,$arr))
            echo "<br>書込に失敗しました";
        else
            echo "<br>データを書込ました";
        fclose($FP);
    }
?>
</HEAD>
<!-- <BODY onLoad=setTimeout("location.href='../index.html'",0)> -->
<!-- <P>�?3秒後にジャンプしま�?(JavaScript�?)</P> -->
</BODY>
</HTML>


