<?php declare(strict_types = 1);

namespace App\Module\Core\Datagrid;

use Ublaboo\DataGrid\DataGrid;

/**
 */
class DataGridFactory
{
    public function create(): DataGrid
    {
        return new DataGrid();
    }
}
