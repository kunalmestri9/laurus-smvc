<?php

import("com.laurus.core.actions.CoreActionHttp");
import("com.laurus.core.services.ServiceManager");
import("com.laurus.core.image.services.api.IImageService");
import("com.laurus.core.image.actions.ActImageAction");

/**
 * Description of ActUpdateImageDirectoryPathInDb
 *
 * @author ABC
 */
class ActUpdateImageDirectoryPathInDb extends ActImageAction {

    protected $logger;

    public function __construct() {
        $this->logger = Logger::getLogger(__CLASS__);
    }

    public function imageInvoke(\IImageService $imageservice, \HttpRequest $wr) {
        $this->logger->info(__METHOD__);
        $list = $wr->getCommandAttribute("upimageBean", true);
        $this->logger->debug($list);
        if ($list && $list instanceof SimpleList) {
            try {
                for ($i = 0; $i < $list->size(); $i++) {
                    $imageservice->updateImageDirectoryPathInDb($list->get($i));
                }
            } catch (SqlException $ex) {
                if ($ex->getCode() == SQLExceptionType::NO_ROWS_AFFECTED) {
                    // throw new ValidationException("File Path can not be updated:: ".$ex->getMessage(),  IKayoExceptionType::UNKNOWN);
                } else {
                    throw new ValidationException("Can not create image :: " . $ex->getMessage(), IParentViewExceptionType::IMAGE_NOT_EXSIST);
                }
            } catch (Exception $e) {
                throw new ValidationException("Opps!! Something went wrong" . $ex->getMessage(), IKayoExceptionType::UNKNOWN);
            }
        }


        //$wr->setJSONResponseBean($list);
    }

}

?>
