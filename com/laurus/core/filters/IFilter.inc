<?php
/**
 * This will help to Declare the RULES associated with the FILTER
 * @author kunal.mestri
 */
import("com.laurus.core.filters.FilterConfig");
import("com.laurus.core.filters.FilterException");

interface IFilter {
    /**
     * throws InvalidRequestException
     * @param HttpRequest $wR
     */
    public function doFilter(HttpRequest $wR);
    public function initFilter(FilterConfig $f);
}

?>
