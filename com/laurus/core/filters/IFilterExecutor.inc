<?php
/**
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
interface IFilterExecutor {
    public function setFiltersConfiguration($config);
    public function executeFilters();
}

?>
