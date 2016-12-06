<?php
import("com.laurus.core.image.services.api.IImageService");
import("com.laurus.core.image.exceptions.*");

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of IImageAction
 *
 * @author Amruta
 */
interface IImageAction {
    public function imageInvoke(\IImageService $imageservice,\HttpRequest $wr);
}

?>
