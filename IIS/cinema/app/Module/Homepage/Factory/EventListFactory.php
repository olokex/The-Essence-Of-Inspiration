<?php declare(strict_types = 1);

namespace App\Module\Homepage\Factory;

use App\Module\Core\Datagrid\DataGridFactory;
use Doctrine\ORM\QueryBuilder;
use Ublaboo\DataGrid\DataGrid;
use Ublaboo\DataGrid\DataSource\DoctrineDataSource;

/**
 */
class EventListFactory
{
    private const
        TITLE = 'title',
        DATE_TIME = 'dateTime',
        TYPE = 'type',
        GENRES = 'genresAsString',
        PERFORMERS = 'performers',
        RATING = 'rating';

    /** @var DataGridFactory */
    private $dataGridFactory;

    public function __construct(DataGridFactory $dataGridFactory)
    {
        $this->dataGridFactory = $dataGridFactory;
    }

    public function create(QueryBuilder $qb): DataGrid
    {
        $grid = $this->dataGridFactory->create();
        $grid->setPagination(false);
        $grid->setDataSource(new DoctrineDataSource($qb, 'e.id'));
        $grid->addColumnLink(self::TITLE, 'Název', 'detail!', 'content.title');
        $grid->addColumnDateTime(self::DATE_TIME, 'Datum a čas')
            ->setFormat('j.n.Y H:i');
        $grid->addColumnText(self::TYPE, 'Typ', 'content.type.title');
        $grid->addColumnText(self::GENRES, 'Žánry', 'content.genresAsString');
        $grid->addColumnText(self::PERFORMERS, 'Účinkující', 'content.performers');
        $grid->addColumnText(self::RATING, 'Hodnocení', 'content.rating');

        return $grid;
    }
}
