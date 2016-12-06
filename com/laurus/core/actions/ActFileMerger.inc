<?php
import("com.laurus.core.actions.CoreActionHttp");

/**
 * Description of ActFileMerger
 *
 * @author Kunal Mestri
 */
class ActFileMerger extends CoreActionHttp{
    public function executeInvoke(\HttpRequest $wr) {
        $filePaths=base64_decode($wr->getParameter("path"));
        $fileArray=split("\\|", $filePaths);
        $str="";
        $type=base64_decode($wr->getParameter("type"));
        foreach ($fileArray as $v) {
            if(TUtilString::equals($type,"text/css"))
                $folderName="css";
            else if (TUtilString::equals($type,"text/javascript"))
                $folderName="js";
            $p=$_SERVER["WEB_PATH"].DIRECTORY_SEPARATOR.$folderName.DIRECTORY_SEPARATOR.$v;
            $fileContent=file_get_contents($p);
            if($fileContent!=FALSE)
                $str.=$fileContent;
            
        }
        header("Content-type: ".$type);
        //$str = preg_replace('/(\/\*([\s\S]*?)\*\/)|(\/\/(.*)$)/m', '', $str);
        //$str= preg_replace('/(?|(?:#|//)([^\r\n]*)|*([\s\S]*?)*/)/', '', $str);
       // $str= preg_replace('~[ \t\n]//[^\r\n]*~', '', $str);
        //$str = trim(preg_replace('/\s+/', ' ', $str));
        echo $str;
    }
}

?>
