<?php declare(strict_types = 1);

namespace App\Module\Hall\Factory;

use App\Module\Core\Datagrid\DataGridFactory;
use App\Module\Hall\Repository\HallRepository;
use App\Module\Hall\Service\HallService;
use Ublaboo\DataGrid\DataGrid;
use Ublaboo\DataGrid\DataSource\DoctrineDataSource;

/**
 */
class HallListFactory
{
    public const
        TITLE = 'title',
        DESCRIPTION = 'description',
        ROW_COUNT = 'rowCount',
        SEAT_IN_ROW_COUNT = 'seatInRowCount';

    /** @var DataGridFactory */
    private $dataGridFactory;

    /** @var HallRepository */
    private $hallRepository;

    public function __construct(DataGridFactory $dataGridFactory, HallRepository $hallRepository)
    {
        $this->dataGridFactory = $dataGridFactory;
        $this->hallRepository = $hallRepository;
    }

    public function create(): DataGrid
    {
        $grid = $this->dataGridFactory->create();
        $grid->setPagination(false);
        $grid->setDataSource(new DoctrineDataSource($this->hallRepository->getQueryBuilder(), 'h.id'));
        $grid->addColumnText(self::TITLE, 'Název');
        $grid->addColumnText(self::DESCRIPTION, 'Popis');
        $grid->addColumnText(self::ROW_COUNT, 'Počet řad', 'rowCount');
        $grid->addColumnText(self::SEAT_IN_ROW_COUNT, 'Počet sedadel v řadě', 'seatInRowCount');

        $grid->addAction('edit!', 'Upravit');

        return $grid;
    }
}
