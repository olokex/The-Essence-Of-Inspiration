<?php declare(strict_types = 1);

namespace App\Module\Content\Factory;

use App\Module\Core\Datagrid\DataGridFactory;
use App\Module\Content\Repository\ContentRepository;
use App\Module\Hall\Repository\HallRepository;
use App\Module\User\Entity\User;
use Ublaboo\DataGrid\DataGrid;
use Ublaboo\DataGrid\DataSource\DoctrineDataSource;

/**
 */
class ContentListFactory
{
    public const
        TITLE = 'title',
        DESCRIPTION = 'description',
        TYPE = 'type',
        GENRES_AS_STRING = 'genresAsString',
        RATING = 'rating',
        LENGTH = 'length';

    /** @var DataGridFactory */
    private $dataGridFactory;

    /** @var ContentRepository */
    private $contentRepository;

    public function __construct(DataGridFactory $dataGridFactory, ContentRepository $contentRepository)
    {
        $this->dataGridFactory = $dataGridFactory;
        $this->contentRepository = $contentRepository;
    }

    public function create(?User $user = null): DataGrid
    {
        $grid = $this->dataGridFactory->create();
        $grid->setPagination(false);
        $grid->setDataSource(new DoctrineDataSource($this->contentRepository->getQueryBuilder(), 'c.id'));
        $grid->addColumnLink(self::TITLE, 'Název', 'detail!');
        $grid->addColumnText(self::DESCRIPTION, 'Popis');
        $grid->addColumnText(self::TYPE, 'Typ', 'type.title');
        $grid->addColumnText(self::GENRES_AS_STRING, 'Žánry');
        $grid->addColumnText(self::RATING, 'Hodnocení');
        $grid->addColumnText(self::LENGTH, 'Délka [min]');

        return $grid;
    }
}
