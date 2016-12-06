<?php
import("com.laurus.core.actions.CoreActionHttp");
import("com.laurus.velox.services.api.IFileSystemService");

/**
 * Description of ActFileUpload
 *
 * @author Kunal
 */
class ActFileUpload extends CoreActionHttp{
    public function executeInvoke(\HttpRequest $wr) {
        $filePath=base64_decode($wr->getHeaderValue("path"));
        $length=$wr->getHeaderValue("Content-Length");
        $streamHandle= fopen("php://input", "r");
        /*@var $mFileSystemService IFileSystemService*/
        $mFileSystemService=ServiceManager::getInstance()->getService(IFileSystemService::NAME);
        $mFileSystemService->appendFile($filePath, $streamHandle, $length);
    }
}

?>
