<?php

/**
 * Description of IFilterHelper
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
interface IFilterHelper {
    public function validateSecurityToken(\HttpRequest $wR,  \SessionValidatorApi $sess);
}

?>
